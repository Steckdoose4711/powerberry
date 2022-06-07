/**
 * @file threadsafe_fifo.cpp
 * @author Florian Atzenhofer
 * @date 29.05.2022
 * @brief This class represents a threadsafe fifo [Implementation]
 */

#include "threadsafe_fifo.h"

int threadsafe_fifo::push(size_t const device_nr, std::shared_ptr<std::vector<measurement_t>> const& samples)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(m_queue.size() >= m_max_size)
        {
            return -1;
        }
        m_queue.push(std::make_tuple(device_nr, samples));
        return 0;
    }
}

size_t threadsafe_fifo::getFillLevel()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.size();
}


std::queue<device_measurement_t> threadsafe_fifo::get_all_measurements()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        // copy the content of the queue to a new queue
        std::queue<device_measurement_t> tmp = m_queue;
        m_queue = std::queue<device_measurement_t>();
        
        // flush queue
        std::queue<device_measurement_t> empty;
        std::swap( m_queue, empty );

        return tmp;
    }
}