/**
 * @file config_Manager.cpp
 * @author Florian Atzenhofer
 * @date 02.11.2021
 * @brief Module to read a json file from the filesystem and get properties[Implementation]
 *
 */
#include "config_Manager.h"
#include <iostream>
#include <fstream>
#include <sstream>

config_Manager::config_Manager(std::string const & path)
{
    m_path_to_config = path;
    m_config = std::make_shared<nlohmann::json>();

    try
    {
        // open config file and serialize it to the configuration
        std::ifstream configstream (path);
        if(configstream.is_open())
        {
            configstream >> *m_config;
        }
        else
        {
            throw "Could not open config file.";
        }
    }
    catch(std::exception const &e)
    {
        throw "[ERROR] Could not open config file";
    }
}

void config_Manager::refreshConfig()
{
    try
    {
        // open config file and serialize it to the configuration
        std::ifstream configstream (m_path_to_config);
        if(configstream.is_open())
        {
            configstream >> *m_config;
        }
        else
        {
            std::cerr << "Could not refresh config file because file was removed from filesystem." << std::endl;
        }
    }
    catch(std::exception const &e)
    {
        std::cerr << "Could not refresh config file because: " << e.what() << std::endl;
    }
}


size_t config_Manager::strToSize_t(std::string const &str)
{
    try
    {
        std::string raw_val = m_config->at("str");
        std::stringstream  sstream(raw_val);
        size_t converted_value = 0;
        sstream >> converted_value;
        return converted_value;
    
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        throw "[ERROR] Could not get measurement rate from config file. Please check config file.";
    }
}

float config_Manager::strTofloat(std::string const &str)
{
    try
    {
        std::string raw_val = m_config->at("str");
        std::stringstream  sstream(raw_val);
        float converted_value = 0;
        sstream >> converted_value;
        return converted_value;
    
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        throw "[ERROR] Could not get measurement rate from config file. Please check config file.";
    }
}



size_t config_Manager::get_NrADCdevices()
{
    return strToSize_t("number_adc_devices");
}

size_t config_Manager::get_NrChannelsPerADC()
{
    return strToSize_t("number_channels_per_device");
}

float config_Manager::get_vRef_V()
{
    return strTofloat("adc_reference_voltage_V");
}


filter_type config_Manager::getFilterType_dsp()
{
    std::string filter_type_str = m_config->at("dsp_filter_type");
    if(filter_type_str == "median")
    {
        return filter_type::median;
    }
    else if(filter_type_str == "mean")
    {
        return filter_type::mean;
    }
    else
    {
        throw "[ERROR] Filter type is not supported (Only 'median' or 'mean' is supported at the moment!). Please check config file.";
    }
}


size_t config_Manager::getMeasurementRate_dsp()
{
    return strToSize_t("measurement_rate_dsp");
}


size_t config_Manager::getSamplingRate_dsp()
{
    return strToSize_t("sampling_rate");
}