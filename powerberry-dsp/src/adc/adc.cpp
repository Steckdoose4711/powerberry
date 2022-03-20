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
#include <math.h>

static uint8_t send_data[8][3] = {
                                {0x06, 0x00 , 0x00 },     // channel0 single ended
                                {0x06, 0x40 , 0x00 },     // channel1 single ended
                                {0x06, 0x80 , 0x00 },     // channel2 single ended     
                                {0x06, 0xC0 , 0x00 },     // channel3 single ended
                                {0x07, 0x00 , 0x00 },     // channel4 single ended
                                {0x07, 0x40 , 0x00 },     // channel5 single ended
                                {0x07, 0x80 , 0x00 },     // channel6 single ended
                                {0x07, 0xC0 , 0x00 }      // channel7 single ended
                                };


adc::adc(std::shared_ptr<spi_wrapper> const spi, size_t const chip_select)
{
    if(spi == nullptr)
    {
        throw "[ERROR]: pointer to SPI must be provided when initializing the ADC!";
    }
    m_spi = spi;
    m_chip_select = chip_select;
    m_v_reference = DEFAULT_REFERENCE_VOLTAGE;
    m_resolution = DEFAULT_RESOLUTION;
}

float adc::read_voltage(size_t const channel)
{
    if(channel > 7)
    {
        throw "[ERROR]: channel must be between 0 and 7!";
    }
        // make them static to do not have to create a new object every time

        static uint8_t read_data[] = {0x00, 0x00 , 0x00 };

        // reading three bytes from the ADC is necessary, because the ADC has a 12 bit resolution and needs some time to sample the voltage
        // the command specifies the number of the channel which is used and is stored in a lookup table for each channel
        m_spi->spi_transfer(send_data[channel], read_data, m_chip_select, sizeof(read_data));

        // merge the read bytes to the measured ADC Digits (0xFFF = 12 bit)
        uint32_t measured_digits = ((read_data[1] << 8) | read_data[2]) & 0x00000FFF;

        float voltage = ((measured_digits * 1.0) / pow(2, m_resolution)) * m_v_reference;
        return voltage;
}


void adc::set_reference_voltage(float const v_reference)
{
    m_v_reference = v_reference;
}