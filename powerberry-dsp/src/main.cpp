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
#include "adc/adc.h"
#include "filters/filter_interface.h"
#include "filters/filter_median.h"
#include "datastorage/datastorage_interface.h"
#include "datastorage/datastorage_redis.h"
#include "controller/controller.h"

using namespace std;
using namespace sw::redis;

#define RELEASE_VERSION 0
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
    std::string config_file_path = "/srv/powerberry/config.json";
    if(argc > 1)
    {
        config_file_path = argv[1];
    }

    //TODO: finish config file implementation

    // Creating instances of the needed DSP Blocks for real ADC
    config_Manager json_config;

    // spi, adc and median filter
    std::shared_ptr<spi_wrapper> spi_wrapper_instance = std::make_shared<spi_wrapper>();
    std::shared_ptr<adc_interface> adc0_instance = std::make_shared<adc>(spi_wrapper_instance, ADC0_Chipselect);
    std::shared_ptr<filter_interface> filter_instance = std::make_shared<filter_median>();

    //redis
    std::shared_ptr<datastorage_interface> datastorage_instance = std::make_shared<datastorage_redis>("tcp://powerberry-redis:6379", 1, 8, 100);

    // controller
    // at first, we have to store our ADCs in a vector, because the controller-interface wants a vector...
    std::vector<std::shared_ptr<adc_interface>> adcs;
    adcs.emplace_back(adc0_instance);
    std::shared_ptr<controller> controller_instance = std::make_shared<controller>(adcs, filter_instance, datastorage_instance);

    json_config.readConfig(config_file_path);


    controller_instance->start_DSP();



    while(true)
    {

    }
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