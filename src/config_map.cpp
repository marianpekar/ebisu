#include "config_map.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

ConfigMap::ConfigMap(const char* config_file_path)
{
    if (!Parse(config_file_path))
    {
        std::cout << "[ConfigMap] Failed to parse config file: " << config_file_path << std::endl;
        throw;
    }
}

bool ConfigMap::Parse(const char* config_file_path)
{
    std::ifstream config_file(config_file_path);

    if (!config_file.is_open())
    {
        return false;
    }

    std::string line;
    while (std::getline(config_file, line))
    {
        if (line.empty())
            continue;
        
        const size_t equal_pos = line.find('=');
        if (equal_pos == std::string::npos)
        {
            std::cout << "[ConfigMap] Invalid property format, expected format is tKey=Value" << std::endl;
            return false;
        }
        std::string key = line.substr(1, equal_pos - 1);
        std::string value = line.substr(equal_pos + 1);

        if (line[0] == 's')
        {
            string_properties.emplace(key, value);
        }
        else if (line[0] == 'i')
        {
            int int_value = std::stoi(value);
            int_properties.emplace(key, int_value);
        }
        else if (line[0] == 'b')
        {
            bool bool_value = StringToBool(value);
            bool_properties.emplace(key, bool_value);
        }
        else if (line[0] == 'f')
        {
            float float_value = std::stof(value);
            float_properties.emplace(key, float_value);
        }
        else
        {
            std::cout << "[ConfigMap] Unknown property type" << std::endl;
            return false;
        }
    }

    config_file.close();
    return true;
}

bool ConfigMap::StringToBool(std::string& str)
{
    std::ranges::transform(str, str.begin(), [](const unsigned char c) { return std::tolower(c); });

    if (str == "true" || str == "1")
        return true;

    if (str == "false" || str == "0")
        return false;

    throw std::invalid_argument("[ConfigMap] Invalid boolean representation: " + str);
}
