/**
 * @file filter_mean.cpp
 * @author Florian Atzenhofer
 * @date 20.03.2022
 * @brief Mean filter element [Implementation]
 *
 * This module provides a filter using the mean.
 */

#include "filter_mean.h"
#include <algorithm>

float filter_mean::filter_values(std::vector<float> values)
{
    // get the mean
    float sum = 0;
    for (auto const& value : values)
    {
        sum += value;
    }
    return sum / values.size();
}

