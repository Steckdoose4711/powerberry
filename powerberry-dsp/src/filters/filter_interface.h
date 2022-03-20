#ifndef FILTER_INTERFACE_H
#define FILTER_INTERFACE_H
/**
 * @file adc_interface.h
 * @author Florian Atzenhofer
 * @date 20.03.2022
 * @brief Interface for Voltage filters
 *
 * This interface allows different filter types for the voltage.
 */

#include <stddef.h>
#include <stdint.h>
#include <vector>

typedef std::tuple <uint64_t, float> measurement_t;


/*! \class FILTER_interface
    \brief Interface for different filters.
*/
class filter_interface
{
    public:
        /**
         * Filters the given voltages and returns one filterd voltage.
         * @param values vector with the raw values (be carefull, this vector is passed by VALUE to implicit copy the vector content)
         * @return the filtered voltage
         */
        virtual measurement_t filter_values(std::vector<measurement_t> values) = 0;
};

#endif // ADC_INTERFACE_H