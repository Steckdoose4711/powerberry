#ifndef DATASTORAGE_H
#define DATASTORAGE_H
/**
 * @file datastorage.h
 * @author Florian Atzenhofer
 * @date 20.03.2022
 * @brief This is the interface of the datastorage[Interface]
 */

#include <stddef.h>
#include <memory>
#include <vector>

#include "filters/filter_interface.h"

class datastorage
{

    public:

        /**
         * Store data for all channels of a devcice to the datastorage
         * @param device_id number of device, which channels should be stored
         * @param measurements all values for this device (all channels)
         * 
         * @return NONE
         */
        virtual void store_measurement(size_t const device_id, std::shared_ptr<std::vector<measurement_t>> const & measurements) = 0;

    private:

        /**
         * Setting the number of Devices for the datastorage
         * @param number_devices number of used decives (ADCs)
         * @return NONE
         */
        virtual void set_nr_devices(size_t const number_devices) = 0;

        /**
         * Setting the number of Channels for the datastorage
         * @param device number of device, which channels should be set
         * @param number_channels number of used ADC channels
         * @return NONE
         */
        virtual void set_nr_channels(size_t const device, size_t const number_channels) = 0;

        /**
         * Setting the number of Samples for the datastorage
         * @param device  number of device, which channels should be set
         * @param channel  channel, which channels should be set
         * @param number_samples sample_frequency of theadc
         * @return NONE
         */
        virtual void set_sample_frequency(size_t const device, size_t const channel, size_t const sample_frequency) = 0;


};

#endif // DATASTORAGE_H