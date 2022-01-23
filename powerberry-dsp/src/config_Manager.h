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
    void readConfig(std::string const &path);
    

    private:
    nlohmann::json j;

};