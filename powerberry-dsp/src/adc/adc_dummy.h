#ifndef ADC_DUMMY_H
#define ADC_DUMMY_H
/**
 * @file adc_dummy.h
 * @author Florian Atzenhofer
 * @date 30.20.2021
 * @brief Module to generate ADC Dummy Values [Interface]
 *
 * This module generates an ideal sinewave and returns it's values
 */
#include "adc_interface.h"
#include <vector>
#include <chrono>



class adc_dummy : public adc_interface
{

    public:

        /**
         * Initialize the ADC .
         * @return errorcode (0) at success (-1) at failure
         */
        adc_dummy();

        /**
         * Read voltages from a specific ADC Channel .
         * @param channel select one channel (from 0 to 7)
         * @return the voltage, which was measured at this channel
         */
        float read_voltage(size_t const channel);

        /**
         * Sets the reference voltage for the ADC. This is needed to convert digits to voltage.
         * @param v_reference reference voltage of the ADC in [V]
         * @return NONE
         */
        void set_reference_voltage(float const v_reference);        

        /**
         * Returns the number of supported channels by this ADC
         * @return number of supported channels
         */
        size_t get_number_channels();

    private:


        /**
         * Returns the time of the current sample point in the current periode of the sinewave
         * @return sample point of the sinewave in milliseconds (maximum value is the periode duration of the simulated sinewave, which is typically 19ms)
         */
        size_t get_time_in_current_periode();

        /**
         * Add noise for the current sample value of the sinewave
         * @param sample the sample value of the sinewave will be noisy after calling this function
         * @return NONE
         */
        void add_noise_to_sample(double & sample);

        size_t m_nr_used_channels = 0;
        std::vector<double> m_sine_wave;
        std::chrono::_V2::steady_clock::time_point m_start;

};

#endif // ADC_DUMMY_H