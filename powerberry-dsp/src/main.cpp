#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <sw/redis++/redis++.h>
#include <bcm2835.h>
#include <memory>

#include "adc/adc_interface.h"
#include "adc/adc_dummy.h"
#include "config_manager/config_Manager.h"
#include "spi/spi_wrapper.h"
#include "adc/adc_MCP3208.h"
#include "filters/filter_interface.h"
#include "filters/filter_median.h"
#include "filters/filter_mean.h"

#include "datastorage/datastorage_interface.h"
#include "datastorage/datastorage_redis.h"
#include "controller/controller.h"

using namespace std;
using namespace sw::redis;

#define ADC0_Chipselect RPI_V2_GPIO_P1_24

int main(int argc, char *argv[])
{

    std::cout << "[INFO]: Hey, Powerberry DSP is up and running." << std::endl;

    std::string config_file_path = "/config/config.json";
    auto env_config_file_path = std::getenv("CONFIG_PATH");
    auto env_redis_host = std::getenv("REDIS_HOST");
    auto env_redis_port = std::getenv("REDIS_PORT");

    // redis must exist
    if(env_redis_host == nullptr || env_redis_port == nullptr)
    {
        throw "[ERROR]: Could not find environment variables REDIS_HOST or REDIS_PORT";
    }

    // check if config file path is set via command line argument
    if(argc > 1)
    {
        config_file_path = argv[1];
    }
    // look if environment variable with path to config file is set   
    else if(env_config_file_path != nullptr)
    {
        config_file_path = env_config_file_path;
    }
    else
    {
        std::cout << "[INFO]: No path to configfile was specified. Using default path: " << config_file_path << std::endl; 
    }

    // Creating instances of the needed DSP Blocks for real ADC
    config_Manager json_config(config_file_path);



    // SPI and ADC
    // at the moment, only one ADC is allowed
    if(json_config.get_NrADCdevices() > 1) throw std::runtime_error("Only one ADC device is allowed at the momend. Extend config file within an array of chipselect pins to allow more than one ADC device.");
    
    std::vector<std::shared_ptr<adc_interface>> adcs;
    // check, if hardware must be emulated
    auto env_emulate_HW = std::getenv("EMULATE_HARDWARE");
    std::string not_emulated("emu_hw_false");
    std::string emulated("emu_hw_true");
    
    if(env_emulate_HW == nullptr || (not_emulated.compare(env_emulate_HW) == 0))
    {
        std::cout << "[INFO]: Using real Hardware" << std::endl;
        // spi
        std::shared_ptr<spi_wrapper> spi_wrapper_instance = std::make_shared<spi_wrapper>();

        // Hardware must not be emulated -> create real ADCs
        for(size_t i = 0; i < json_config.get_NrADCdevices(); i++)
        {
            std::shared_ptr<adc_interface> adc_instance = std::make_shared<adc_MCP3208>(spi_wrapper_instance, ADC0_Chipselect, json_config.get_vRef_V());
            adcs.emplace_back(adc_instance);
        }
    }
    else if(emulated.compare(env_emulate_HW) == 0)
    {
        std::cout << "[INFO]: Emulating Hardware" << std::endl;
        // we have to emulate the hardware
                // Hardware must not be emulated -> create real ADCs
        for(size_t i = 0; i < json_config.get_NrADCdevices(); i++)
        {
            std::shared_ptr<adc_interface> adc_instance = std::make_shared<adc_dummy>();
            adcs.emplace_back(adc_instance);
        }
    }
    else
    {
        throw std::runtime_error("Environment variable EMULATE_HARDWARE must be set to 'emu_hw_true' or 'emu_hw_false'.");
    }

    // filter
    std::shared_ptr<filter_interface> filter_instance;
    if(json_config.getFilterType_dsp() == filter_type::mean)
    {
        filter_instance = std::make_shared<filter_mean>();
    }
    else if(json_config.getFilterType_dsp() == filter_type::median)
    {
        filter_instance = std::make_shared<filter_median>();
    }

    // redis
    std::string redis_connectionstring = "tcp://" + std::string(env_redis_host) + ":" + std::string(env_redis_port);
    /*
    std::shared_ptr<datastorage_interface> datastorage_instance = std::make_shared<datastorage_redis>(redis_connectionstring, 
                                                                                                      json_config.get_NrADCdevices(), 
                                                                                                      json_config.get_NrChannelsPerADC(),
                                                                                                      json_config.getSamplingRate_dsp());

    // controller
    std::shared_ptr<controller> controller_instance = std::make_shared<controller>( adcs, 
                                                                                    filter_instance, 
                                                                                    datastorage_instance,
                                                                                    json_config.getMeasurementRate_dsp(),
                                                                                    json_config.getSamplingRate_dsp());

    controller_instance->start_DSP();
*/

    std::cout << "[INFO]: Bye, Powerberry DSP is shutting down..." << std::endl;

}