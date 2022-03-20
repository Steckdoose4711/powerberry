/**
 * @file adc.h
 * @author Florian Atzenhofer
 * @date 20.03.2022
 * @brief Abstraction of an ADC [Implementation]
 *
 * This module provides the functioonality to read voltages from an ADC.
 */
#include "adc.h"

#include <stdlib.h>

adc::adc(spi_wrapper * const spi)
{
    if(spi == nullptr)
    {
        throw "[ERROR]: pointer to SPI must be provided when initializing the ADC!";
    }
    m_spi = spi;
}

float adc::read_voltage(size_t const channel)
{
    return 0.0f;
}
