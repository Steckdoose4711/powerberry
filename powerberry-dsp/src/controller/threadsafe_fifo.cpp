/**
 * @file threadsafe_fifo.cpp
 * @author Florian Atzenhofer
 * @date 29.05.2022
 * @brief This class represents a threadsafe fifo [Implementation]
 */

#include "threadsafe_fifo.h"

int threadsafe_fifo::push(size_t device_nr, std::vector<measurement_t> & samples)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        
        
        // add a vector with channels for each ADC
        while((m_p_channelVectors.get())->size() <= device_nr)
        {
            m_p_channelVectors->emplace_back(std::make_shared<Channel_Sample_t>());
        }

        // size of samples is equal to the channel number because samples contains one sample for each channel
        auto nr_channels = samples.size();

        // add a vector with samples for each channel
        auto p_adc = (m_p_channelVectors.get())[device_nr];
        while(p_adc.size() <= nr_channels)
        {
            p_adc.emplace_back(std::make_shared<Sample_t>);
        }
/*

        if((*m_channelVectors.get())[device_nr][samples.size()].size() >= m_max_size)
        {
            return -1;
        }

        size_t cnt = 0;
        for(auto it = samples.begin(); it != samples.end(); it++)
        {
            (*m_channelVectors.get())[device_nr][cnt].emplace_back(samples[cnt]);
            cnt++;
        }
*/
        return 0;
    }
}

size_t threadsafe_fifo::getFillLevel()
{
    //std::lock_guard<std::mutex> lock(m_mutex);
    return 0;
    //return m_channelVectors.size();
}


ADC_Channel_Sample_t threadsafe_fifo::pop_all_measurements()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
/*
        auto p_temp = m_p_channelVectors;
        m_p_channelVectors = std::make_shared<ADC_Channel_Sample_t>();

        return p_temp;
        */
    }
}