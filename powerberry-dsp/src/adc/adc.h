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
#include <memory>
#include "spi/spi_wrapper.h"


#define DEFAULT_RESOLUTION 12           // the default resolution of the ADC is 12 Bit
#define DEFAULT_REFERENCE_VOLTAGE 4.8   // the default reference voltage of the ADC is 4.8 V

class adc : public adc_interface
{

    public:
        /**
         * Initialize the ADC.
         * @param spi pointer to the SPI wrapper, which is needed to communicate with the ADC
         * @param chip_select pin for the chip select of the ADC
         */
        adc(std::shared_ptr<spi_wrapper> const spi, size_t const chip_select);

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


    private:

    // private members
    std::shared_ptr<spi_wrapper> m_spi;     //!< pointer to the SPI wrapper, which is needed to communicate with the ADC
    float m_v_reference;                   //!< reference voltage of the ADC in [V]
    size_t m_resolution;                    //!< resolution of the ADC in [Bit]
    size_t m_chip_select;                   //!< chip select pin for the ADC

};

#endif // ADC_H