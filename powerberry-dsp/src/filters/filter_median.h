#ifndef FILTER_MEDIAN_H
#define FILTER_MEDIAN_H
/**
 * @file filter_median.h
 * @author Florian Atzenhofer
 * @date 20.03.2022
 * @brief Median filter element [Interface]
 *
 * This module provides a filter using the median.
 */
#include <vector>
#include <memory>

#include "filter_interface.h"

class filter_median : public filter_interface
{


    public:
        /**
         * Filters the given voltages using the median value of the vector elements and returns one filterd voltage.
         * @param values vector with the raw values (be carefull, this vector is passed by VALUE)
         * @return the filtered voltage
         */
        float filter_values(std::vector<float> values);

    private:

};

#endif // FILTER_MEDIAN_H