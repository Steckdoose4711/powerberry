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

// at the first run, read the complete file
void read_config_initialiy(std::string const & path, nlohmann::json & config);

// at the second and all other run, only refresh the changeable values
void refreshConfig(std::string const & path, nlohmann::json & config);


// refresh config
void config_Manager::readConfig()
{
    if(m_path_to_config.empty())
    {
        std::cout << "No path to config file given. Using default path." << std::endl;
        m_path_to_config = "./config.json";
    }
    readConfig(m_path_to_config);
}

// read config initialy from file or refresh config
void config_Manager::readConfig(std::string const &path)
{
    // test json library
    if(m_path_to_config.empty())
    {
        m_path_to_config = path;
        read_config_initialiy(m_path_to_config, m_config);
    }
    else
    {
        refreshConfig(m_path_to_config, m_config);
    }
}

void read_config_initialiy(std::string const & path, nlohmann::json & config)
{
    try
    {
        // open config file and serialize it to the configuration
        std::ifstream configstream (path);
        if(configstream.is_open())
        {
            configstream >> config;
        }
        else
        {
            std::cout << "Could not open config file." << std::endl;
        }
    }
    catch(std::exception const &e)
    {
        std::cout << "Error while reading config file: " << e.what() << std::endl;
    }
}


void refreshConfig(std::string const & path, nlohmann::json & config)
{
        try
    {
        // open config file and serialize it to the configuration
        std::ifstream configstream (path);
        if(configstream.is_open())
        {
            nlohmann::json config_new;
            configstream >> config_new;
            config["measurement_rate_dsp"] = config_new["measurement_rate_dsp"];
            config["sampling_rate_dsp"] = config_new["sampling_rate_dsp"];
            config["voltage_reference_V"] = config_new["voltage_reference_V"];

        }
        else
        {
            std::cout << "Could not open config file." << std::endl;
        }
    }
    catch(std::exception const &e)
    {
        std::cout << "Error while reading config file: " << e.what() << std::endl;
    }
}