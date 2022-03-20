#ifndef DATASTORAGE_REDIS_H
#define DATASTORAGE_REDIS_H
/**
 * @file datastorage_redis.h
 * @author Florian Atzenhofer
 * @date 20.03.2022
 * @brief This is the interface of the redis datastorage[Interface]
 */

#include "datastorage_interface.h"

#include <sw/redis++/redis++.h>
#include <string>
#include <memory>
#include <vector>

using namespace sw::redis;

class datastorage_redis : public datastorage_interface
{
    public:

        /**
         * Initialize redis with default parameters
         * @return NONE
         */
        datastorage_redis();


        /**
         * Initialize redis with custom parameters
         * @param connectionstring redis connection string
         * @param number_devices number of used decives (ADCs)
         * @param number_channels number of used ADC channels (at the moment, each ADC must have the same number of channels)
         * @param sample_frequency sample_frequency of adc
         * @return NONE
         */
        datastorage_redis(std::string const & connectionstring, size_t const number_devices, size_t const number_channels, size_t const sample_frequency);


        /**
         * Store data for all channels of a devcice to the datastorage
         * @param device_id number of device, which channels should be stored
         * @param measurements all values for this device (all channels)
         * 
         * @return NONE
         */
        void store_measurement(size_t const device_id, std::shared_ptr<std::vector<measurement_t>> const & measurements);

    private:

        /**
         * Setting the number of Devices for the datastorage
         * @param number_devices number of used decives (ADCs)
         * @return NONE
         */
        void set_nr_devices(size_t const number_devices);

        /**
         * Setting the number of Channels for the datastorage
         * @param device number of device, which channels should be set
         * @param number_channels number of used ADC channels
         * @return NONE
         */
        void set_nr_channels(size_t const device, size_t const number_channels);

        /**
         * Setting the number of Samples for the datastorage
         * @param device  number of device, which channels should be set
         * @param channel  channel, which channels should be set
         * @param number_samples sample_frequency of theadc
         * @return NONE
         */
        void set_sample_frequency(size_t const device, size_t const channel, size_t const sample_frequency);

        /**
         * Setting the connectionstring to the redis server
         * @param connectionstring connectionstring to the redis server
         * @return NONE
         */
        void set_connection_string(std::string const & connectionstring);    

    std::string m_connectionstring;
    std::shared_ptr<Redis> m_redis;
    bool m_is_initialized = false;  // only initialize redis once because multiple times would crash the channels/devices/frequency mapping

};

#endif // DATASTORAGE_REDIS_H