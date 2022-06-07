#ifndef CONTROLLER_H
#define CONTROLLER_H
/**
 * @file controller.h
 * @author Florian Atzenhofer
 * @date 20.03.2022
 * @brief This class controlls the measurtement of the ADC and pushes the filtered values to Redis [Interface]
 */

#include <vector>
#include <memory>
#include <tuple>

#include "threadsafe_fifo.h"
#include "adc/adc_interface.h"
#include "filters/filter_interface.h"
#include "datastorage/datastorage_interface.h"

class controller
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
        controller( std::vector<std::shared_ptr<adc_interface>> adc_list,
                    std::shared_ptr<filter_interface> p_filter,
                    std::shared_ptr<datastorage_interface> p_datastorage,
                    size_t const measurement_rate,
                    size_t const sampling_rate);

        /**
         * This function reads the ADC Samples for each channel and pushes the filtered values to Redis.
         * @return none
         */
        void start_DSP();


    private:

        /**
         * This function pushes the filtered values to Redis in a seperate thread.
         * @return NONE
         */
    void pushDataToDatastorage();


    std::vector<std::shared_ptr<adc_interface>> m_adc_list;
    std::shared_ptr<filter_interface> m_p_filter;
    std::shared_ptr<datastorage_interface> m_p_datastorage;
    size_t m_measurement_rate;
    size_t m_sampling_rate;
    threadsafe_fifo m_cache_fifo;
};

#endif // CONTROLLER_H