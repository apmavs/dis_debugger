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
    config_values[CONFIG::METADATA_XML] = "dis_definitions.xml";
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

// Return value from tag <Tag>value</Tag>
// Returns empty string if not found
std::string Configuration::getTagValue(std::string fromStr, std::string tag)
{
    std::string ret("");
    std::string beginTag("<" + tag + ">");
    std::string endTag("</" + tag + ">");
    size_t begin = fromStr.find(beginTag);
    if(begin != std::string::npos)
    {
        begin     += beginTag.length();
        // Find the end position of end tag that matches open tag
        size_t end = std::string::npos;
        int level = 0;
        for(size_t pos = begin; pos < fromStr.length(); pos++)
        {
            // Make sure we don't find a duplicate tag from a child
            // that had the same class type
            if((level == 0) && (fromStr.substr(pos).find(endTag) == 0))
            {
                end = pos;
                break;
            }
            else if(fromStr.substr(pos, endTag.length()) == endTag)
                level -= 1;
            else if(fromStr.substr(pos, beginTag.length()) == beginTag)
                level += 1;
        }

        if((end != std::string::npos) && (end > begin))
        {
            size_t tagLen = end - begin;
            ret = fromStr.substr(begin, tagLen);
        }
        else if((end == std::string::npos) || (end < begin))
            std::cerr << "Could not find " << endTag << " in:" << fromStr << std::endl;
    }
    return ret;
}

