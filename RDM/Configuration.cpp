#include "Configuration.h"

#include <fstream>

int Configuration::CONFIG_ELEMENTS = sizeof(CONFIG::ConfigNames) /
                                     sizeof(CONFIG::ConfigNames[0]);

Configuration::Configuration() :
    file_name("rdm.config")
{
    // Set defaults
    config_values[CONFIG::BROADCAST_IP] = "127.0.0.1";
    config_values[CONFIG::BROADCAST_PORT] = "6993";
    config_values[CONFIG::METADATA_XML] = "C:\\Comp\\school_work\\CSE\\dis_debugger\\dis_definitions.xml";
    loadConfigFile(file_name);
    saveConfig(); // Create config file if it did not exist
}

Configuration* Configuration::instance = NULL;
Configuration* Configuration::getInstance()
{
    if(instance == NULL)
        instance = new Configuration();

    return instance;
}

Configuration::~Configuration()
{
}

std::string Configuration::getValue(CONFIG::ConfigValue type)
{
    return config_values[type];
}

bool Configuration::setValue(CONFIG::ConfigValue type,
              std::string value,
              bool persistent)
{
    bool success = false;
    if(type < CONFIG_ELEMENTS)
    {
        config_values[type] = value;
        success = true;
    }

    if(persistent && success)
        saveConfig();

    return success;
}

bool Configuration::loadConfigFile(std::string fileName)
{
    bool success = false;
    std::string line;
    std::ifstream readFile(fileName.c_str());
    if(readFile.is_open())
    {
        while(getline(readFile, line))
        {
            // Find strings before and after = sign
            std::string::size_type eqPos = line.find("=");
            std::string key = line.substr(0, eqPos);
            std::string val = line.substr(eqPos+1);
            for(int i = 0; i < CONFIG_ELEMENTS; i++)
            {
                if(CONFIG::ConfigNames[i] == key)
                {
                    config_values[CONFIG::ConfigValue(i)] = val;
                    success = true;
                    break;
                }
            }
        }
        readFile.close();
    }
    return success;
}

bool Configuration::saveConfig()
{
    bool success = false;
    std::ofstream writeFile(file_name.c_str());
    if(writeFile.is_open())
    {
        std::map<CONFIG::ConfigValue, std::string>::const_iterator it;
        for(it = config_values.begin(); it != config_values.end(); it++)
        {
            writeFile << CONFIG::ConfigNames[it->first];
            writeFile << "=" << it->second << "\n";
        }
        writeFile.close();
        success = true;
    }
    return success;
}

