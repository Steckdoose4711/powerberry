#ifndef SPI_WRAPPER_H
#define SPI_WRAPPER_H

/**
 * @file spi_wrapper.h
 * @author Florian Atzenhofer
 * @date 19.03.2022
 * @brief Module to encapsulate SPI calls [Interface]
 *
 */
#include <vector>
#include <mutex>

class spi_wrapper
{
    public:
        /**
         * SPI CTOR.
         * @param chip_select sets the default GPIO Pin, which is used for the Chip select of the ADC.
         */
        spi_wrapper(size_t const chip_select);

        /**
         * SPI CTOR which is using the defaule CS pin (PIN24 = GPIO8).
         */
        spi_wrapper();        

          /**
         * Initialize the SPI.
         * @param[in] tbuf Buffer of bytes to send. 
         * @param[out] rbuf Received bytes will by put in this buffer
         * @param [in] chip_select Chip Select pin, which is used by this instance.
         * @param[in] len Number of bytes in the tbuf buffer, and the number of bytes to send/received
         * @return errorcode (0) at success (-1) at failure
         */ 
        void spi_transfer(char * const tbuf, char * const rbuf, size_t const chip_select,  size_t const len);


    private:

        /**
         * release all allocated SPI-Resources
         */
        ~spi_wrapper();

    size_t m_current_cs_pin;
    std::mutex m_spi_mutex; // locks multiple calls of the spi against each other

};

#endif // ADC_DUMMY_H