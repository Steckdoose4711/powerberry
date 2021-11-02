/**
 * @file read_config.h
 * @author Florian Atzenhofer
 * @date 02.11.2021
 * @brief Module to read a json file from the filesystem [Interface]
 *
 */
#include <string>

class Read_config
{
    public:
    std::string getConfig(std::string const &path);
    

};