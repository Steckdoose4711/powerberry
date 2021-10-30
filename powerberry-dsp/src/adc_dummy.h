/**
 * @file adc_dummy.h
 * @author Florian Atzenhofer
 * @date 30.20.2021
 * @brief Module to generate ADC Dummy Values [Interface]
 *
 * This module generates an ideal sinewave and returns it's values
 */
#include "adc_interface.h"


void testfunc();


class adc_dummy : public adc_interface
{

    public:
        /**
         * Initialize the ADC .
         * @param chip_select sets the GPIO Pin, which is used for the Chip select of the ADC.
         * @return errorcode (0) at success (-1) at failure
         */
        int Init(size_t const chip_select);

        /**
         * Read voltages from a specific ADC Channel .
         * @param channel select one channel (from 0 to 7)
         * @return the voltage, which was measured at this channel
         */
        float read_voltage(size_t const channel);

};