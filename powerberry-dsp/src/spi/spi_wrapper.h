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

// Defines
#define DEFAULT_SPI_PIN 24



class spi_wrapper
{
    public:

        /**
         * SPI CTOR uses the defaule CS pin (PIN24 = GPIO8).
         */
        spi_wrapper();

          /**
         * Transfers data through the SPI bus. This function is thread safe.
         * @param[in] tbuf Buffer of bytes to send. 
         * @param[out] rbuf Received bytes will by put in this buffer
         * @param [in] chip_select Chip Select pin, which is used by this instance.
         * @param[in] len Number of bytes in the tbuf buffer, and the number of bytes to send/received
         * @return errorcode (0) at success (-1) at failure
         */ 
        void spi_transfer(uint8_t * const tbuf, uint8_t * const rbuf, size_t const chip_select,  size_t const len);

        /**
         * release all allocated SPI-Resources
         */
        virtual ~spi_wrapper();

    private:



    // private members
    size_t m_current_cs_pin; // current chip select pin
    std::mutex m_spi_mutex; // locks multiple calls of the spi against each other

};

#endif // ADC_DUMMY_H