#pragma once
#include <json.hpp>

class JsonWriter
{
public:
    static void SaveLevelToFile(const char* file_path, const class Editor* editor);
private:
    static void SerializeBasicMapData(const Editor* editor, nlohmann::json& json);
    static void SerializeTileMapLayers(const Editor* editor, nlohmann::json& json);
    static void SerializeCollisionMap(const Editor* editor, nlohmann::json& json);
    static void SerializeEntities(const Editor* editor, nlohmann::json& json);
    
    static void SaveStringToFile(const char* file_path, const std::string& json_string);
};
