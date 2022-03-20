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

float filter_median::filter_values(std::vector<float> values)
{
    // sort the values
    std::sort(values.begin(), values.end());

    // get the median
    size_t const median_index = values.size() / 2;
    return values[median_index];
}

