/**
 * @file adc_interface.h
 * @author Florian Atzenhofer
 * @date 30.20.2021
 * @brief Interface for ADC Access
 *
 * This interface allows it to use a dummy for the ADC.
 * With the dummy, it is not necessary to execute the code on a real Hardware.
 */

#include <stddef.h>
#include <stdint.h>


/*! \class SPI_interface
    \brief Wrapper for the SPI.

    SPI can be either a real hardware or a mudule, which returns dummy data.
*/
class adc_interface
{

    public:
        /**
         * Initialize the ADC .
         * @param chip_select sets the GPIO Pin, which is used for the Chip select of the ADC.
         * @return errorcode (0) at success (-1) at failure
         */
        virtual int init(size_t const chip_select) = 0;

        /**
         * Read voltages from a specific ADC Channel .
         * @param channel select one channel (from 0 to 7)
         * @return the voltage, which was measured at this channel
         */
        virtual float read_voltage(size_t const channel) = 0;

};