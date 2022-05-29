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
        threadsafe_fifo();



    private:

};

#endif // CONTROLLER_H