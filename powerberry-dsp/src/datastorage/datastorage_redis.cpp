/**
 * @file datastorage_redis.cpp
 * @author Florian Atzenhofer
 * @date 20.03.2022
 * @brief This is the implementation of the redis datastorage[Implementation]
 */


#include "datastorage_redis.h"

#include <iostream>

datastorage_redis::datastorage_redis()
{
    set_connection_string("tcp://powerberry-redis:6379");
    set_nr_devices(1);
    size_t nr_channels = 8;
    set_nr_channels(0, nr_channels);

    for(size_t channel = 0; channel < nr_channels; channel++)
    {
        set_sample_frequency(0, channel, 100);
    }
}


datastorage_redis::datastorage_redis(std::string const & connectionstring, size_t const number_devices, size_t const number_channels, size_t const sample_frequency)
{
    set_connection_string(connectionstring);
    set_nr_devices(number_devices);

    // set number of channel for each device
    for(size_t i = 0; i < number_devices; i++)
    {
        set_nr_channels(i, number_channels);
    }

    // set sample frequency for each channel in each device
    for(size_t device = 0; device < number_devices; device++)
    {
        for(size_t channel = 0; channel < number_channels; channel++)
        {
            set_sample_frequency(device, channel, sample_frequency);
        }
    }
}

 void datastorage_redis::store_measurement(size_t const device_id, std::shared_ptr<std::vector<measurement_t>> const & measurements)
{
    // build string for redis
    size_t channel_number = 0;
    for(auto it = measurements->begin(); it != measurements->end(); it++)
    {
        uint64_t timestamp = std::get<0>(*it);
        float value = std::get<1>(*it);
        std::string measurement_str= std::to_string(timestamp) + ";" + std::to_string(value);

        std::string redis_str = "device:" + std::to_string(device_id) + ":channel:" + std::to_string(channel_number) + ":current";
        m_redis->lpush(redis_str, measurement_str);
        channel_number++;
    }

    //m_redis->lpush("device:0:channel:0:current", ss.str());
}



void datastorage_redis::set_nr_devices(size_t const number_devices)
{
    if(m_is_initialized)
    {
        std::cout << "[ERROR:] Redis is already initialized" << std::endl;
        return;
    }   

    // build string for adding multiple devices
    std::string dev = "";
    for(size_t i = 0; i < number_devices; i++)
    {
        if(i != 0)  dev += " ";
        dev += std::to_string(i);
    }

    m_redis->sadd("devices", dev);
}

void datastorage_redis::set_nr_channels(size_t const device, size_t const number_channels)
{
    if(m_is_initialized)
    {
        std::cout << "[ERROR:] Redis is already initialized" << std::endl;
        return;
    }   

    std::string channels = "";
    for(size_t i = 0; i < number_channels; i++)
    {
        if(i != 0)  channels += " ";
        channels += std::to_string(i);
    }
    
    m_redis->sadd("device:" + std::to_string(device) + ":channels", channels);
}

void datastorage_redis::set_sample_frequency(size_t const device, size_t const channel, size_t const sample_frequency)
{
    if(m_is_initialized)
    {
        std::cout << "[ERROR:] Redis is already initialized" << std::endl;
        return;
    }   

    m_redis->set("device:" + std::to_string(device) + ":channel:" + std::to_string(channel) + ":sample_rate", std::to_string(sample_frequency));
}

void datastorage_redis::set_connection_string(std::string const & connectionstring)
{
    m_connectionstring = connectionstring;
    m_redis = std::make_unique<Redis>(m_connectionstring);
}
