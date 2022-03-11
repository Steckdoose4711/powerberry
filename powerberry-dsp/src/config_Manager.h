/**
 * @file config_Manager.h
 * @author Florian Atzenhofer
 * @date 02.11.2021
 * @brief Module to read a json file from the filesystem and get properties [Interface]
 *
 */
#include <string>
#include "json.hpp"


class config_Manager
{
    public:
    // read config initialy from file or refresh config
    void readConfig(std::string const &path);

    // refresh config
    void readConfig();
    

    private:
    nlohmann::json m_config;
    std::string m_path_to_config;

};