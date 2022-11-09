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
        if(device_nr > 0)
        {
            throw std::runtime_error("[ERROR]: Only one ADC is supported at this time!");
        }

        std::lock_guard<std::mutex> lock(m_mutex);

        // for each channel, add a vector for it's measurement
        while(m_p_channels->size() <= samples.size())
        {
            m_p_channels->emplace_back(std::make_shared<Samples_t>());
        }


        bool max_size_reached = false;

        // Check, if one of the vector already reached it's max values
        /*
        for(size_t channel = 0; channel < samples.size(); channel++)
        {
            if((m_p_channels.get())->at(channel)->size() >= m_max_measurements_per_channel)
            {
                max_size_reached = true;
            }
        }
*/
        // do not add further values, if maximum size of the buffer is already reached
        if(max_size_reached)
        {
            return -1;
        }

        // add one sample for each channel
        for(size_t channel = 0; channel < samples.size(); channel++)
        {
            measurement_t measurement = samples[channel];
            auto channelValues = m_p_channels->at(channel);
            channelValues->emplace_back(measurement);
        }

        //success
        return 0;
    }
}

size_t threadsafe_fifo::getFillLevel()
{
    //std::lock_guard<std::mutex> lock(m_mutex);
    return 0;
    //return m_channelVectors.size();
}


std::shared_ptr<Channel_Sample_t> threadsafe_fifo::pop_all_measurements()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
/*
        auto p_temp = m_p_channelVectors;
        m_p_channelVectors = std::make_shared<pADCs_Channels_Samples_t>();

        return p_temp;
        */
    }
}