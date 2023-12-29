#include "json_reader.h"

#include <fstream>
#include <json.hpp>

#include "editor.h"
#include "entity.h"

using json = nlohmann::json;

void JsonReader::LoadLevelFromFile(const char* assets_path, const char* file_path, Editor* editor)
{
    std::ifstream level_file(file_path);

    const json map_data = json::parse(level_file);

    float tile_size = map_data["TileSize"];
    editor->SetTileSize(tile_size);

    int rows = map_data["Rows"];
    editor->SetRowTileCount(rows);

    int cols = map_data["Columns"];
    editor->SetColumnTileCount(cols);

    std::vector<int> collision_map = map_data["CollisionMap"];
    editor->SetCollisionMap(collision_map);

    for (auto& tilemap_layers = map_data["TileMapLayers"]; const json& tilemap_layer_json : tilemap_layers)
    {
        std::string sheet_path = tilemap_layer_json["SpriteSheet"];
        bool is_front = tilemap_layer_json["IsFront"];
        std::vector<int> tilemap_data = tilemap_layer_json["TileMap"];
        editor->AddTilemapLayer(sheet_path.c_str(), is_front, tilemap_data);
    }
    
    for (auto& entities = map_data["Entities"]; const json& entity_json : entities)
    {
        std::string entity_name = entity_json["Name"];
        const int entity_id = entity_json["Id"];
        const bool is_active = entity_json["IsActive"];

        auto* entity = new Entity(entity_name.c_str(), entity_id);
        entity->is_active = is_active;
        
        for (auto& components = entity_json["Components"]; const json& component : components)
        {
            const auto& component_type = component["Type"];

            if (component_type == "MapCollider")
            {
                int width = component["Width"];
                int height = component["Height"];
                entity->AddComponent<MapCollider>(width, height);
            }
            if (component_type == "BoxCollider")
            {
                int width = component["Width"];
                int height = component["Height"];
                entity->AddComponent<BoxCollider>(width, height);
            }
            if (component_type == "SpriteSheet")
            {
                int width = component["Width"];
                int height = component["Height"];
                std::string sheet_file_path = component["FilePath"];
                entity->AddComponent<SpriteSheet>(std::format("{}/{}", assets_path, sheet_file_path), width, height);
            }
            if (component_type == "Rigidbody")
            {
                const float mass = component["Mass"];
                const float drag = component["Drag"];
                entity->AddComponent<Rigidbody>(mass, drag);   
            }
            if (component_type == "Transform")
            {
                const float x = component["X"];
                const float y = component["Y"];
                Component* transform = entity->GetComponent("Transform");
                transform->SetFloat("X", x);
                transform->SetFloat("Y", y);
            }
            if (component_type == "Animator")
            {
                if (component.contains("Animation"))
                {
                    const auto& animation = component["Animation"];
                    const int row = animation["SpriteSheetRow"];
                    const int start_frame = animation["StartFrame"];
                    const int end_frame = animation["EndFrame"];
                    const int frame_time = animation["FrameTime"];
                    const bool is_loop = animation["IsLoop"];
                    const bool play_on_setup = animation["PlayOnSetup"];
                    entity->AddComponent<Animator>(row, start_frame, end_frame, frame_time, is_loop, play_on_setup);
                }
            }
            if (component_type == "Agent")
            {
                const float speed = component["Speed"];
                const float stopping_distance = component["StoppingDistance"];
                const int target_entity_id = component["TargetEntityId"];
                entity->AddComponent<Agent>(speed, stopping_distance, target_entity_id);
            }
            if (component_type == "PlayerController")
            {
                entity->AddComponent<PlayerController>();
            }
            if (component_type == "MainCamera")
            {
                entity->AddComponent<MainCamera>();
            }
            if (component_type == "CharacterAnimator")
            {
                const float idle_anim_frame_time = component["IdleAnimationFrameTime"];
                const int idle_anim_start_frame = component["IdleAnimationStartFrame"];
                const int idle_anim_end_frame = component["IdleAnimationEndFrame"];
                const float idle_to_move_anim_threshold = component["IdleToMoveAnimationThreshold"];
                const float move_anim_frame_time = component["MoveAnimationFrameTime"];
                const int move_anim_start_frame = component["MoveAnimationStartFrame"];
                const int move_anim_end_frame = component["MoveAnimationEndFrame"];
                entity->AddComponent<CharacterAnimator>(idle_anim_frame_time, idle_anim_start_frame, idle_anim_end_frame,
                    idle_to_move_anim_threshold, move_anim_frame_time, move_anim_start_frame, move_anim_end_frame);
            }
        }

        editor->AddEntityAt(entity, entity_id);
    }
}
