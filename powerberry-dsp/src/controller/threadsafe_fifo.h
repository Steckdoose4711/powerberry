#ifndef THREADSAFE_FIFO_H
#define THREADSAFE_FIFO_H
/**
 * @file threadsafe_fifo.h
 * @author Florian Atzenhofer
 * @date 29.05.2022
 * @brief This class represents a threadsafe fifo [Interface]
 */

#include <vector>
#include <memory>
#include <tuple>
#include <queue>
#include <mutex>

#include "filters/filter_interface.h"

typedef std::vector<measurement_t> Samples_t;
typedef std::vector<std::shared_ptr<Samples_t>> Channel_Sample_t;


class threadsafe_fifo
{
    public:

        /**
         * This is the class, which controlls the measurtement of the ADC and pushes the filtered values to Redis
         * @param adc_list vector contains all the adcs
         * @param p_filter vector contains the filter, which is used by the dsp
         * @param p_datastorage pointer to the datastorage, which is used by the dsp
         * @param measurement_rate measurement rate of the adc (number of measurements per sample)
         * @param sampling_rate sampling frequency of the DSP
         * @return the filtered voltage
         */
        threadsafe_fifo()
        {
            m_p_channels = std::make_shared<Channel_Sample_t>();

        };

    /*
          Tfree complete memory which was occupied by this module
         
        ~threadsafe_fifo()
        {
            for(auto it = m_p_channels->begin(); it != m_p_channels->end(); it++)
            {
                delete &it;
            }
            delete m_p_channels;
        }
*/
        /**
         * Push a measurement for a device to the fifo
         * @param device_nr number of the measuring device
         * @param samples samples for the channels of the device
         * @return 0, if the measurement was pushed to the fifo, -1, if the fifo is full or blocked
         */
        int push(size_t device_nr, std::vector<measurement_t> & samples);

        /**
         * Returns the fill level of the fifo
         * @return fill level
         */
        size_t getFillLevel();

        /**
         * Get all Measurements from the fifo
         * @return vector with all the measurements
         */
         std::shared_ptr<Channel_Sample_t> pop_all_measurements();


    private:

    std::shared_ptr<Channel_Sample_t> m_p_channels;
    std::mutex m_mutex;
    size_t m_max_measurements_per_channel = 100000;

};

#endif // CONTROLLER_H