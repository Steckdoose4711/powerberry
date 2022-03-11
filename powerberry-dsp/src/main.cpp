#include <iostream>
#include <chrono>
#include <thread>
#include "adc_dummy.h"
#include <sw/redis++/redis++.h>
#include "config_Manager.h"

using namespace std;
using namespace sw::redis;

#define RELEASE_VERSION 1

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
int main(int argc, char *argv[])
{

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

    // read json config file
    config_Manager json_config;
    json_config.readConfig(config_file_path);

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

    adc_interface * test = new adc_dummy();

    size_t const testchannel = 4;

    test->init(0);


    while(true)
    {
        for(int i = 0; i < 100; i++)
        {
            std::ostringstream ss;
            auto sample_point = test->read_voltage(testchannel);
            ss << sample_point;
            redis.lpush("device:0:channel:0:current", ss.str());

            //std::cout << "sample point: " << sample_point << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        redis.ltrim("device:0:channel:0:current", 0, 10000);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        auto len = redis.llen("device:0:channel:0:current");
        cout << len<< std::endl;
    }
}
#endif