/**
 * @file controller.cpp
 * @author Florian Atzenhofer
 * @date 20.03.2022
 * @brief This class controlls the measurtement of the ADC and pushes the filtered values to Redis [Implementation]
 */

#include "controller.h"


#include<thread>
#include <chrono>
#include <iostream>
#include "threadsafe_fifo.h"

#define ENABLE_DEBUG_INFOS 0


 static threadsafe_fifo cache_fifo;

/**
 * This function pushes the filtered values to Redis in a seperate thread.
 * @return NONE
 */
static void TransferCacheToRedis(std::shared_ptr<datastorage_interface> p_datastorage);

controller::controller( std::vector<std::shared_ptr<adc_interface>> adc_list,
                        std::shared_ptr<filter_interface> p_filter,
                        std::shared_ptr<datastorage_interface> p_datastorage,
                        size_t const measurement_rate,
                        size_t const sampling_rate)
{
    m_adc_list = adc_list;
    m_p_filter = p_filter;
    m_p_datastorage = p_datastorage;
    m_measurement_rate = measurement_rate;
    m_sampling_rate = sampling_rate;
}


void controller::start_DSP()
{
    // [warning] This is a fire & forget thread
    std::thread t1(TransferCacheToRedis, m_p_datastorage);

    while(true)
    {
        // we need to measure the time, which is needed for our code to calculate waiting time until the next measurement
        auto start = std::chrono::high_resolution_clock::now();
        size_t used_device = 0;

        std::chrono::duration<int64_t, std::nano> time_alloc;
        std::chrono::duration<long, std::nano>  time_measure;
        std::chrono::duration<long, std::nano>  time_filter;
        std::chrono::duration<long, std::nano>  time_store;

        // do measurement for all ADCs
        for (auto const& adc : m_adc_list)
        {
            std::vector<std::vector<measurement_t>> raw_values;
            std::vector<measurement_t> p_filtered_adc_values;

            // we need to know how much channel this adc has
            size_t number_channels = adc->get_number_channels();

            auto now1 = std::chrono::high_resolution_clock::now();
            time_alloc = (now1 - start);

            // do measurement for all channels in this ADC
            for(size_t channel = 0; channel < number_channels; channel++)
            {
                std::vector<measurement_t> raw_channel_values;
                for(size_t sample_cnt = 0; sample_cnt < m_measurement_rate; sample_cnt++)
                {
                    uint64_t timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();    // timestamp in mictoseconds since epoch
                    float voltage = adc->read_voltage(channel); //voltage of the channel in [V]
                    measurement_t measurement(timestamp, voltage);

                    raw_channel_values.emplace_back(measurement);
                    
                }
                raw_values.emplace_back(raw_channel_values);
            }

            auto now = std::chrono::high_resolution_clock::now();
            time_measure = (now - now1);

            // Do filtering for all channels of this ADC
            // filtered_adc_values will have same size as number of channels of this ADC
            for(auto it = raw_values.begin(); it != raw_values.end(); it++)
            {
                p_filtered_adc_values.emplace_back(m_p_filter->filter_values(*it)); // filter values for each channel
            }




            now1 = std::chrono::high_resolution_clock::now();
            time_filter = (now1 - now);

            // now we have filtered all channels of this ADC and we can store them into our data storage
            // each entry is a sample for a channel (e.g. p_filtered_adc_values[0] is the sample for channel 0, p_filtered_adc_values[1] is the sample for channel 1)
            cache_fifo.push(used_device, p_filtered_adc_values);
            used_device++;

            now = std::chrono::high_resolution_clock::now();
            time_store = (now - now1);

        }

        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed_ms = (end - start);

        #if ENABLE_DEBUG_INFOS == 1
            std::cout << "alloc  : " << time_alloc.count() /1000.0 << " us" << std::endl;
            std::cout << "measure: " << time_measure.count() /1000.0 << " us"  << std::endl;
            std::cout << "filter : " << time_filter.count() /1000.0 << " us"  << std::endl;
            std::cout << "store  : " << time_store.count() /1000.0 << " us"  << std::endl;
            std::cout << "sum    : " << elapsed_ms.count() /1000.0 << " us"  << std::endl;
            std::cout << "-----------------------------------" << std::endl;
        #endif
        
        size_t periode_time_ns = (1.0 / m_sampling_rate) * 1000000000;
        auto timeToWait = std::chrono::nanoseconds(periode_time_ns) - std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed_ms);
        if(timeToWait > std::chrono::milliseconds::zero())
        {
            std::this_thread::sleep_for(timeToWait);
        }
        else
        {
            std::cerr << "[WARNING] to slow to process all samples" << std::endl;
        }
    }
}

#if ENABLE_DEBUG_INFOS == 1
    static size_t test = 0;
#endif
static void TransferCacheToRedis(std::shared_ptr<datastorage_interface> p_datastorage)
{
    while(true)
    {
        // get the complete content from the queue and copy each measurement into redis seperately
        auto measurements = cache_fifo.pop_all_measurements();

        #if ENABLE_DEBUG_INFOS == 1
            if(measurements.size() > test)
            {
                std::cout << "size (" << measurements.size() << ") is " << measurements.size() - test << " greater than last time(" << test << ")" << std::endl;
            }
            test = measurements.size();
        #endif



            // store to redis
            std::cout << "Storing " + measurements->size() << " elements" << std::endl;

                for(size_t channel = 0; channel < (measurements.get())->size(); channel++)
                {
                    auto samples = *(measurements.get()->at(channel).get());
                    p_datastorage->store_measurement(0, channel, samples);
                }

        // we allow one second delay between measurement and viasualizing the data
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }
}