/**
 * @file datastorage_redis.cpp
 * @author Florian Atzenhofer
 * @date 20.03.2022
 * @brief This is the implementation of the redis datastorage[Implementation]
 */


#include "datastorage_redis.h"

#include <iostream>
#include <chrono>
#include <thread>

#define ENABLE_DEBUG_INFOS 1



datastorage_redis::datastorage_redis()
{
    datastorage_redis("tcp://powerberry-redis:6379", 1, 3,100);
}


datastorage_redis::datastorage_redis(std::string const & connectionstring, size_t const number_devices, size_t const number_channels, size_t const sample_frequency)
{
    bool connectionSucceeded = false;
    size_t trycounter = 0;
    while(!connectionSucceeded)
    {
        try
        {
            set_connection_string(connectionstring);
            set_nr_devices(number_devices);
            connectionSucceeded = true;
            std::cout << "[Info] Connection to redis server '" << connectionstring << "' established" << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << "[ERROR] Try(" << trycounter << "). Redis cannot be reached. " << e.what() << std::endl;
            trycounter++;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

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

        #if ENABLE_DEBUG_INFOS == 1
            std::cout << redis_str << " " << measurement_str << std::endl;
        #endif
        m_redis->lpush(redis_str, measurement_str);
        channel_number++;
    }
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
