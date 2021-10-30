#include <iostream>
#include <chrono>
#include <thread>
#include "adc_dummy.h"

using namespace std;

int main()
{

    adc_interface * test = new adc_dummy();

    size_t const testchannel = 4;
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        cout << test->read_voltage(testchannel) << endl;
    }

    return 0;
}
