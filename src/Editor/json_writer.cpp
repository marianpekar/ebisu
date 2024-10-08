#include "editor.h"
#include "json_writer.h"

#include <fstream>
#include <iostream>

void JsonWriter::SaveLevelToFile(const char* file_path, const Editor* editor)
{
    nlohmann::json json;

    SerializeBasicMapData(editor, json);
    SerializeCollisionMap(editor, json);
    SerializeTileMapLayers(editor, json);
    SerializeEntities(editor, json);
    
    SaveStringToFile(file_path, json.dump());
}

void JsonWriter::SerializeBasicMapData(const Editor* editor, nlohmann::json& json)
{
    json["TileSize"] = editor->GetTileSize();
    json["Rows"] = editor->GetRowTileCount();
    json["Columns"] = editor->GetColumnTileCount();
}

void JsonWriter::SerializeTileMapLayers(const Editor* editor, nlohmann::json& json)
{
    json["TileMapLayers"] = nlohmann::json::array();
    
    const std::vector<TilemapLayer> tilemap_layers = editor->GetTilemapLayers();
    std::vector<std::string> tilemap_paths = editor->GetTilemapPaths();

    for (size_t i = 0; i < tilemap_layers.size(); i++)
    {
        nlohmann::json tilemap_layer_entry;
        tilemap_layer_entry["SpriteSheet"] = tilemap_paths[i];
        tilemap_layer_entry["IsFront"] = tilemap_layers[i].is_front;
        tilemap_layer_entry["TileMap"] = tilemap_layers[i].data;

        json["TileMapLayers"].push_back(tilemap_layer_entry);
    }
}

void JsonWriter::SerializeCollisionMap(const Editor* editor, nlohmann::json& json)
{
    json["CollisionMap"] = editor->GetCollisionMap();
}

void JsonWriter::SerializeEntities(const Editor* editor, nlohmann::json& json)
{
    json["Entities"] = nlohmann::json::array();
    
    const std::vector<Entity*> entities = editor->GetEntities();
    for (size_t i = 0; i < entities.size(); i++)
    {
        nlohmann::json entity_entry;
        entity_entry["Id"] = entities[i]->GetId();
        entity_entry["Name"] = entities[i]->GetName();
        entity_entry["IsActive"] = entities[i]->is_active;
        entity_entry["IsPersistent"] = entities[i]->is_persistent;
        
        for (Component* component : entities[i]->GetComponents())
        {
            nlohmann::json component_entry;

            component_entry["Name"] = component->GetName();
            
            if (component->GetName() == "Animator")
            {
                SerializeAnimationProperties(component, component_entry);
            }
            else
            {
                SerializeProperties(component, component_entry);
            }

            entity_entry["Components"] = component_entry;
        }
        
        json["Entities"].push_back(entity_entry);
    }
}

void JsonWriter::SerializeProperties(Component* component, nlohmann::json& entry)
{
    for(const auto& [key, value] : component->float_properties)
    {
        entry[key] = value;
    }

    for(const auto& [key, value] : component->int_properties)
    {
        entry[key] = value;
    }

    for(const auto& [key, value] : component->bool_properties)
    {
        entry[key] = value;
    }

    for(const auto& [key, value] : component->string_properties)
    {
        entry[key] = value;
    }

    for(const auto& [key, value] : component->path_properties)
    {
        entry[key] = value;
    }
}

void JsonWriter::SerializeAnimationProperties(Component* component, nlohmann::json& component_entry)
{
    nlohmann::json animation_entry;
    SerializeProperties(component, animation_entry);
    component_entry["Animation"] = animation_entry;
}

void JsonWriter::SaveStringToFile(const char* file_path, const std::string& json_string)
{
    std::ofstream file_stream(file_path);
    if (file_stream.is_open())
    {
        file_stream << json_string;
        file_stream.close();
    }
    else
    {
        std::cerr << "[JsonWriter::SaveLevelToFile] Error: Unable to save file" << std::endl;
    }
}
