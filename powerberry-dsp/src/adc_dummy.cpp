/**
 * @file adc_dummy.h
 * @author Florian Atzenhofer
 * @date 30.20.2021
 * @brief Module to generate ADC Dummy Values [Implementation]
 *
 * This module generates an ideal sinewave and returns it's values
 */
#include "adc_dummy.h"

int adc_dummy::Init(size_t const chip_select)
{
    return 0;
}


float adc_dummy::read_voltage(size_t const channel)
{
    return 47.11f;
}