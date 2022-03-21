/**
 * @file config_Manager.h
 * @author Florian Atzenhofer
 * @date 02.11.2021
 * @brief Module to read a json file from the filesystem and get properties [Interface]
 *
 */
#include "json.hpp"
#include "filters/filter_interface.h"

#include <string>
#include <memory>

class config_Manager
{
    public:

    /**
     * Creates a new config_Manager
     * @param path path to the config file
     * 
     * @return NONE
     */
    config_Manager(std::string const &path);

    /**
     * Refreshes the config, if the file has changed (must be called somewere periodically)
     * 
     * @return NONE
     */
    void refreshConfig();

    /**
     * Returns the number of used ADC devices for this application
     * 
     * @return number of used adc devices
     */
    size_t get_NrADCdevices();

    /**
     * Returns the number of Channels per ADC device
     * 
     * @return number of used channels per adc
     */
    size_t get_NrChannelsPerADC();

    /**
     * Returns the reference voltage of the ADC
     * 
     * @return reference voltage of the ADC in V
     */
    float get_vRef_V();



    /**
     * Returns the filter type of the DSP
     * 
     * @return type of the dsp filter (median or mean are supported at the moment)
     */
    filter_type getFilterType_dsp();    
    

    /**
     * Returns the name of the influx host
     * 
     * @return measurements per sample (needed for filtering purposes)
     */
    size_t getMeasurementRate_dsp();


    /**
     * Returns the sampling frequency of the DSP
     * 
     * @return sample frequency in HZ
     */
    size_t getSamplingRate_dsp();

    private:

    /**
     * Converts a variable from a config file to size_t
     * @param str name of the variable in the config file
     * 
     * @return value of the variable in the config file
     */
    size_t strToSize_t(std::string const &str);

    /**
     * Converts a variable from string to float
     * @param str name of the variable in the config file
     * 
     * @return value of the variable in the config file
     */
    float strTofloat(std::string const &str);


    std::shared_ptr<nlohmann::json> m_config;
    std::string m_path_to_config;

};