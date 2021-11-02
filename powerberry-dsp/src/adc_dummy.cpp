/**
 * @file adc_dummy.h
 * @author Florian Atzenhofer
 * @date 30.20.2021
 * @brief Module to generate ADC Dummy Values [Implementation]
 *
 * This module generates an ideal sinewave and returns it's values
 */
#include "adc_dummy.h"

#include <math.h>
#include <iostream>
#include <stdlib.h>

    static size_t nr_samples = 100;
    static double current_time_radiant = 0;
    static double max_amplitude = 0.5;
    static size_t sine_wave_frequency = 50;

int adc_dummy::init(size_t const chip_select)
{
    // calculating an ideal sine wave with a resolution of 


    // calculate one ideal sinewave 
    for(size_t i = 0; i < nr_samples; i++)
    {
        double datapoint = max_amplitude * sin(current_time_radiant);
        m_sine_wave.emplace_back(datapoint);
        current_time_radiant += ((2 * M_PI) / nr_samples);
    }

   // get current cpu tick timestamp to simulate the "oscillating" sine wave when reading from it
   m_start = std::chrono::steady_clock::now();
    return 0;
}



float adc_dummy::read_voltage(size_t const channel)
{
    // at first, we have to get the time for the current periode of the sinewave
    size_t time_in_this_periode = get_time_in_current_periode();

    double duration_periode = (1.0 / sine_wave_frequency * 1.0) * 1000;
    size_t index = (nr_samples / duration_periode) * time_in_this_periode;
    
    // now we know the sample point of the sine wave for the current time
    double sample_point = m_sine_wave[index];

    std::cout << "time: " << time_in_this_periode << std::endl;
    std::cout << "index: " << index << std::endl;
    std::cout << "original sample point: " << sample_point << std::endl;
    // now we add a noise to the sampled adc value to simulate measurement inaccuracy
    add_noise_to_sample(sample_point);
    std::cout << "noisy sample point: " << sample_point << std::endl;

    return sample_point;
}

size_t adc_dummy::get_time_in_current_periode()
{
    // get sample time of the sinewave
    auto duration_since_start = std::chrono::steady_clock::now() - m_start;
    auto diff = std::chrono::duration <double, std::milli> (duration_since_start).count();
    size_t rounded_diff = round(diff);                                      // time since start in milliseconds
    size_t periode_time = (1.0f / (sine_wave_frequency * 1.0f)) * 1000.0f;  // periode time in milliseconds

    // this is the sample point of the sinewave
    size_t time_in_this_periode = rounded_diff % periode_time;
    return time_in_this_periode;
}

void adc_dummy::add_noise_to_sample(double & sample)
{
    if(sample == 0) return; // no noise will be added to a sample of 0V

    // generate noise for sampling point to simulate a realistig measured adc value
    int max_val = 100;
    double scaler = 0.05;        // noise can be 5% of the amplitude of the measured signal

    int randVal = rand() % (2 * max_val);
    double noise = randVal - max_val;    // get random value between -100 and 100
    double max_sample = sample * scaler;
    double scaled_noise = noise / (max_val * 1.0f / max_sample);

    // add noise to sample
    sample += scaled_noise;

}