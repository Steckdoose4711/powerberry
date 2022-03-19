/**
 * @file spi_wrapper.cpp
 * @author Florian Atzenhofer
 * @date 19.03.2022
 * @brief Module to encapsulate SPI calls [Interface]
 *
 */
#include "spi_wrapper.h"
#include <bcm2835.h>


spi_wrapper::spi_wrapper()
{
    spi_wrapper(24);
}

spi_wrapper::spi_wrapper(size_t const chip_select)
{
    m_current_cs_pin = chip_select;

    if (!bcm2835_init())
    {
      throw "bcm2835_init failed. Are you running as root??";
    }

    if (!bcm2835_spi_begin())
    {
      throw "bcm2835_init failed. Are you running as root??";
    }

    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128);   //3.125MHz
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default
    bcm2835_spi_chipSelect(m_current_cs_pin);
}

spi_wrapper::~spi_wrapper()
{
    bcm2835_spi_end();
    bcm2835_close();
}


void spi_wrapper::spi_transfer(char * const tbuf, char * const rbuf, size_t const chip_select,  size_t const len)
{
    std::lock_guard<std::mutex> lock(m_spi_mutex);
    if(m_current_cs_pin != chip_select)
    {
        m_current_cs_pin = chip_select;
        bcm2835_spi_chipSelect(chip_select);
    }
    bcm2835_spi_transfernb(tbuf, rbuf, len);
}