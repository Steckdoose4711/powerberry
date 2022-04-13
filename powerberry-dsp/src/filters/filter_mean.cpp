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
#include<tuple>

measurement_t filter_mean::filter_values(std::vector<measurement_t> values)
{

    // get the mean
    float sum_timestamp = 0;
    float sum_value = 0;
    for (auto it = values.begin(); it != values.end(); it++)
    {
        sum_timestamp += std::get<0>(*it);
        sum_value += std::get<1>(*it);
    }
    float mean_timestamp = sum_timestamp / values.size();
    float mean_value = sum_value / values.size();

    return measurement_t(mean_timestamp, mean_value);
}

