/**
 * @file controller.cpp
 * @author Florian Atzenhofer
 * @date 20.03.2022
 * @brief This class controlls the measurtement of the ADC and pushes the filtered values to Redis [Implementation]
 */

#include "controller.h"


#include<thread>
#include <chrono>



controller::controller(std::vector<std::shared_ptr<adc_interface>> adc_list, std::shared_ptr<filter_interface> filter)
{
    m_adc_list = adc_list;
    m_filter = filter;
}


void controller::start_DSP()
{

while(true)
{
    // we need to measure the time, which is needed for our code to calculate waiting time until the next measurement
    auto start = std::chrono::steady_clock::now();
    
    // do measurement for all ADCs
    for (auto const& adc : m_adc_list)
    {
        std::vector<std::vector<measurement_t>> raw_values;
        std::vector<float> filtered_adc_values;

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
            //filtered_adc_values.emplace_back(m_filter->filter_values(*it)); // filter values for each samples
        }

    }

    auto end = std::chrono::steady_clock::now();
    auto elapsed = end - start;

    auto timeToWait = std::chrono::milliseconds(10) - elapsed;
    if(timeToWait > std::chrono::milliseconds::zero())
    {
        std::this_thread::sleep_for(timeToWait);
    }
}
}


