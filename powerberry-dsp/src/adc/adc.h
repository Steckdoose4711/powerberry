#ifndef ADC_H
#define ADC_H
/**
 * @file adc.h
 * @author Florian Atzenhofer
 * @date 20.03.2022
 * @brief Abstraction of an ADC [Interface]
 *
 * This module provides the functioonality to read voltages from an ADC.
 */
#include "adc_interface.h"
#include <vector>
#include <chrono>
#include "spi/spi_wrapper.h"


class adc : public adc_interface
{

    public:
        /**
         * Initialize the ADC.
         * @param spi pointer to the SPI wrapper, which is needed to communicate with the ADC
         */
        adc(spi_wrapper * const spi);

        /**
         * Read voltages from a specific ADC Channel .
         * @param channel select one channel (from 0 to 7)
         * @return the voltage, which was measured at this channel
         */
        float read_voltage(size_t const channel);

    private:

    // private members
    spi_wrapper * m_spi;

};

#endif // ADC_H