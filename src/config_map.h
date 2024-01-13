#pragma once
#include <string>
#include <unordered_map>

class ConfigMap
{
public:
    ConfigMap(const char* config_file_path);

    float GetFloat(const std::string& property_name) { return float_properties[property_name]; }
    int GetInt(const std::string& property_name) { return int_properties[property_name]; }
    bool GetBool(const std::string& property_name) { return bool_properties[property_name]; }
    std::string GetString(const std::string& property_name) { return string_properties[property_name]; }
private:
    std::unordered_map<std::string, float> float_properties;
    std::unordered_map<std::string, bool> bool_properties;
    std::unordered_map<std::string, int> int_properties;
    std::unordered_map<std::string, std::string> string_properties;
    bool Parse(const char* config_file_path);
    static bool StringToBool(std::string& str);
};


