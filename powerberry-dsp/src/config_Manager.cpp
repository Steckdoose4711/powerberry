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


void config_Manager::readConfig(std::string const &path)
{

    // test json library


    std::ifstream config ("/app/src/config.json");
    if(config.is_open())
    {
        config >> j;
    }

    std::string testjson = j.dump();
    std::cout << testjson << std::endl;

}