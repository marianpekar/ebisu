#include "json_reader.h"

#include <fstream>
#include <json.hpp>

using json = nlohmann::json;

void JsonReader::LoadLevelFromFile(const char* file_path, Editor* editor)
{
    std::ifstream level_file(file_path);

    const json map_data = json::parse(level_file);

    for (auto& entities = map_data["Entities"]; const json& entity : entities)
    {
        std::string entity_name = entity["Name"];
        const int entity_id = entity["Id"];
    }
}
