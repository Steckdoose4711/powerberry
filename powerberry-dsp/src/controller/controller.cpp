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



controller::controller(std::vector<std::shared_ptr<adc_interface>> adc_list, std::shared_ptr<filter_interface> p_filter, std::shared_ptr<datastorage_interface> p_datastorage)
{
    m_adc_list = adc_list;
    m_p_filter = p_filter;
    m_p_datastorage = p_datastorage;
}


void controller::start_DSP()
{

while(true)
{
    // we need to measure the time, which is needed for our code to calculate waiting time until the next measurement
    auto start = std::chrono::steady_clock::now();
    size_t used_device = 0;

    // do measurement for all ADCs
    for (auto const& adc : m_adc_list)
    {
        std::vector<std::vector<measurement_t>> raw_values;
        std::shared_ptr<std::vector<measurement_t>> p_filtered_adc_values = std::make_shared<std::vector<measurement_t>>();

        // we need to know how much channel this adc has
        size_t number_channels = adc->get_number_channels();

        // do measurement for all channels in this ADC
        for(size_t i = 0; i < number_channels; i++)
        {
            std::vector<measurement_t> raw_channel_values;
            for(size_t j = 0; j < SAMPLES_PER_CHANNEL; j++)
            {
                uint64_t timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();    // timestamp in mictoseconds since epoch
                float voltage = adc->read_voltage(i); //voltage of the channel in [V]
                measurement_t measurement(timestamp, voltage);

                raw_channel_values.emplace_back(measurement);
                
            }
            raw_values.emplace_back(raw_channel_values);
        }

        // Do filtering for all channels of this ADC
        // filtered_adc_values fill have same size as number of channels of this ADC
        for(auto it = raw_values.begin(); it != raw_values.end(); it++)
        {
            p_filtered_adc_values->emplace_back(m_p_filter->filter_values(*it)); // filter values for each channel
        }

        // now we have filtered all channels of this ADC and we can store them into our data storage
        m_p_datastorage->store_measurement(used_device, p_filtered_adc_values);
        used_device++;
    }

    auto end = std::chrono::steady_clock::now();
    auto elapsed = end - start;

    size_t periode_time = (1.0 / MEASUREMENT_FREQUENCY) * 1000;
    auto timeToWait = std::chrono::milliseconds(periode_time) - elapsed;
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


