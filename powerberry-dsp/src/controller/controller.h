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

#include "adc/adc_interface.h"
#include "filters/filter_interface.h"

#define SAMPLES_PER_CHANNEL 3   // number of samples per measurement for one channel


class controller
{
    public:
        /**
         * Filters the given voltages using the mean value of the vector elements and returns one filterd voltage.
         * @param values vector with the raw values (be carefull, this vector is passed by VALUE)
         * @return the filtered voltage
         */
        controller(std::vector<std::shared_ptr<adc_interface>> adc_list, std::shared_ptr<filter_interface> filter);




    private:

        /**
         * This function reads the ADC Samples for each channel and pushes the filtered values to Redis.
         * @return none
         */
        void start_DSP();

    std::vector<std::shared_ptr<adc_interface>> m_adc_list;
    std::shared_ptr<filter_interface> m_filter;
};

#endif // CONTROLLER_H