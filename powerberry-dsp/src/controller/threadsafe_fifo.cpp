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

        auto tmp = (m_channelVectors.get())->size();

        while(tmp <= device_nr)
        {
            m_channelVectors->emplace_back(new std::vector<std::vector<measurement_t>>);
        }


        while((*m_channelVectors.get())[device_nr].size() <= samples.size())
        {
            (*m_channelVectors.get())[device_nr].emplace_back(new std::vector<measurement_t>);
        }

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

        return 0;
    }
}

size_t threadsafe_fifo::getFillLevel()
{
    //std::lock_guard<std::mutex> lock(m_mutex);
    return 0;
    //return m_channelVectors.size();
}


std::shared_ptr<ADC_Ch_Meas_Vec_t> threadsafe_fifo::pop_all_measurements()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto p_temp = m_channelVectors;
        m_channelVectors = std::make_shared<ADC_Ch_Meas_Vec_t>();

        return p_temp;
    }
}