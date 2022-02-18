#include <iostream>
#include <chrono>
#include <thread>
#include "adc_dummy.h"
#include <sw/redis++/redis++.h>

using namespace std;
using namespace sw::redis;

int main()
{

    // test redis client
    auto redis = Redis("tcp://localhost:6379");


    adc_interface * test = new adc_dummy();

    size_t const testchannel = 4;

    test->init(0);

    while(true)
    {
        for(int i = 0; i < 100; i++)
        {
            auto sample_point = test->read_voltage(testchannel);
            std::cout << "sample point: " << sample_point << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    }


    return 0;
}
