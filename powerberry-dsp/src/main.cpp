#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <sw/redis++/redis++.h>
#include <bcm2835.h>
#include <memory>

#include "adc/adc_interface.h"
#include "adc/adc_dummy.h"
#include "config_Manager.h"
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

#define RELEASE_VERSION 1
#define ADC0_Chipselect RPI_V2_GPIO_P1_18

#if RELEASE_VERSION == 0
/**
    * Do only call this method for testing purposes.
    * Function generates sinusoidal values and pushes them to redis
    * @param NONE
    * @return NONE
*/
static void DSP_Test();
#endif

#if RELEASE_VERSION == 1
/**
    * Do call this function in release version of DSP application.
    * Function reads Values from ADC and deploys them to redis.
    * @param inout args from main program
    * @return NONE
*/
static void DSP_Deploy(int argc, char *argv[]);
#endif

/**
    * Main function
    * @param arg[0] = path to app (implicit parameter)
    *        arg[1] = path to config file (optional)
    * @return NONE
*/

int SPI_Test();

int main(int argc, char *argv[])
{
    //SPI_Test();

    #if (RELEASE_VERSION == 1)
        DSP_Deploy(argc, argv);
    #else
        DSP_Test();
    #endif
    return 0;
}

#if RELEASE_VERSION == 1
static void DSP_Deploy(int argc, char *argv[])
{
    std::string config_file_path = "/config/config.json";
    auto env_config_file_path = std::getenv("CONFIG_PATH");
    auto env_redis_host = std::getenv("REDIS_HOST");
    auto env_redis_port = std::getenv("REDIS_PORT");

    // redis must exist
    if(env_redis_host == nullptr || env_redis_port == nullptr)
    {
        std::cerr << "Could not find environment variables REDIS_HOST or REDIS_PORT" << std::endl;
        return;
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
        throw std::runtime_error("Environment variable EMULATE_HARDWARE must be set to true or false.");
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

}
#endif

#if RELEASE_VERSION == 0
static void DSP_Test()
{

    // test redis client
    auto redis = Redis("tcp://powerberry-redis:6379");

    redis.sadd("devices", "0");

    redis.sadd("device:0:channels", "0");

    redis.set("device:0:channel:0:sample_rate", "100");

    adc_interface * syntheticADC = new adc_dummy();

    size_t const testchannel = 4;



    while(true)
    {
        for(int i = 0; i < 100; i++)
        {
            std::ostringstream ss;
            auto sample_point = syntheticADC->read_voltage(testchannel);
            ss << sample_point;
            redis.lpush("device:0:channel:0:current", ss.str());

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        redis.ltrim("device:0:channel:0:current", 0, 10000);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        auto len = redis.llen("device:0:channel:0:current");
        cout << len<< std::endl;
    }
    
}
#endif



/*
int SPI_Test(){

// If you call this, it will not actually access the GPIO
// Use for testing
//        bcm2835_set_debug(1);

    if (!bcm2835_init())
    {
      printf("bcm2835_init failed. Are you running as root??\n");
      return 1;
    }

    if (!bcm2835_spi_begin())
    {
      printf("bcm2835_spi_begin failed. Are you running as root??\n");
      return 1;
    }
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128);   //3.125MHz
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default

    std::ofstream myfile;
    myfile.open ("samples.csv");


    for(int i = 0; i < 10000; i++)
    {
        char send_data[] = {0x06, 0x00 , 0x00 };
        char read_data[] = {0x00, 0x00 , 0x00 };

        bcm2835_spi_transfernb(send_data, read_data, sizeof(read_data));

        uint32_t retVal = ((read_data[1] << 8) | read_data[2]) & 0x00000FFF;
        double voltage = ((retVal * 1.0) / 4096) * 4.8;

        //std::cout << "b0:" << unsigned(read_data[0]) << " b1:" << unsigned(read_data[1]) << " b2:" << unsigned(read_data[2]) << std::endl;
        std::cout << "voltage = " << voltage << "V" << std::endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
        myfile << voltage << std::endl;

    } 
    // Send a byte to the slave and simultaneously read a byte back from the slave
    // If you tie MISO to MOSI, you should read back what was sent
    //uint8_t send_data = 0x23;
    //uint8_t read_data = bcm2835_spi_transfer(send_data);
  myfile.close();

    bcm2835_spi_end();
    bcm2835_close();
    return 0;
    
}
*/