#ifndef FILTER_MEAN_H
#define FILTER_MEAN_H
/**
 * @file filter_mean.h
 * @author Florian Atzenhofer
 * @date 20.03.2022
 * @brief Mean filter element [Interface]
 *
 * This module provides a filter using the mean value.
 */
#include <vector>
#include <memory>

#include "filter_interface.h"

class filter_mean : public filter_interface
{
    public:
        /**
         * Filters the given voltages using the mean value of the vector elements and returns one filterd voltage.
         * @param values vector with the raw values (be carefull, this vector is passed by VALUE)
         * @return the filtered voltage
         */
        float filter_values(std::vector<float> values);

    private:

};

#endif // FILTER_MEAN_H