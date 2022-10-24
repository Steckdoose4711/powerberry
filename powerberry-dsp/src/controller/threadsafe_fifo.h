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

typedef std::vector<std::vector<std::vector<measurement_t>>> ADC_Ch_Meas_Vec_t;


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
            m_channelVectors = std::make_shared<ADC_Ch_Meas_Vec_t>();

        };

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
        std::shared_ptr<ADC_Ch_Meas_Vec_t> pop_all_measurements();


    private:

    std::shared_ptr<ADC_Ch_Meas_Vec_t> m_channelVectors;
    std::mutex m_mutex;
    size_t m_max_size = 100000;

};

#endif // CONTROLLER_H