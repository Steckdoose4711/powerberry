/**
 * @file filter_median.cpp
 * @author Florian Atzenhofer
 * @date 20.03.2022
 * @brief Median filter element [Implementation]
 *
 * This module provides a filter using the median.
 */

#include "filter_median.h"
#include <algorithm>
#include <tuple>


// Comparison function to sort the vector elements
// by second element of tuples
static bool sortbysec(const measurement_t & a, 
               const measurement_t & b)
{
    return (std::get<1>(a) < std::get<1>(b));
}


measurement_t filter_median::filter_values(std::vector<measurement_t> values)
{
    // sort the measurements by the measured values (not by timestamp)
    std::sort(values.begin(), values.end(), sortbysec);

    // get the median
    size_t const median_index = values.size() / 2;
    return values[median_index];
}

