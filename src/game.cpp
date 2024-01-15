#include <fstream>
#include <iostream>
#include <string>
#include <SDL.h>
#include "game.h"
#include "map.h"
#include "renderer.h"
#include "ECS/collision_solver.h"
#include "ECS/component_manager.h"
#include "ECS/entity.h"
#include "ECS/entity_pool.h"
#include "ECS/Components/agent.h"
#include "ECS/Components/animator.h"
#include "ECS/Components/character_animator.h"
#include "ECS/Components/box_collider.h"
#include "ECS/Components/map_collider.h"
#include "ECS/Components/player_controller.h"
#include "ECS/Components/rigidbody.h"
#include "ECS/Components/sprite_sheet.h"
#include "ECS/Components/transform.h"
#include "ECS/Components/camera.h"
#include "ECS/Components/map_exit.h"

bool Game::Initialize(const char* title, const int screen_width, const int screen_height, const bool fullscreen, const char* map_path, const char* assets_root_path, const char* project_root_path)
{
    assets_path = std::format("{}/{}", project_root_path, assets_root_path);
    
    if (!Renderer::Initialize(title, screen_width, screen_height, fullscreen))
        return false;

    InitializeGameLogicEssentials(screen_width, screen_height);
    if (!LoadLevel(map_path))
    {
        std::cout << "[Game] Failed to load level" << std::endl;
        return false;
    }

    is_running = true;
    
    return true;
}

void Game::InitializeGameLogicEssentials(const int screen_width, const int screen_height)
{
    component_manager = new ComponentManager();
    collision_solver = new CollisionSolver(0, 0, static_cast<float>(screen_width), static_cast<float>(screen_height));

    Renderer::SetMainCamera(new Camera(screen_width, screen_height));
    entity_pool = new EntityPool();
}

bool Game::LoadLevel(const char* map_path)
{
    std::ifstream map_file(std::format("{}/{}", assets_path, map_path));
    if (!map_file.is_open())
        return false;

    const json map_data = json::parse(map_file);
    LoadMap(map_data);
    LoadEntities(map_data);
    
    return true;
}

void Game::LoadMap(const json& map_data)
{
    const std::vector<int> collision_map = map_data["CollisionMap"];

    const int tile_size = map_data["TileSize"];
    const int rows = map_data["Rows"];
    const int cols = map_data["Columns"];
    const int map_width = tile_size * cols;
    const int map_height = tile_size * rows;

    map = new Map(tile_size, map_width, map_height, collision_map);
    
    const auto& layers = map_data["TileMapLayers"];
    for (const auto& layer : layers)
    {
        const std::string tilemap_sheet_path = layer["SpriteSheet"];
        const std::vector<int> tile_map = layer["TileMap"];
        const bool is_front = layer["IsFront"];
        map->AddLayer(std::format("{}/{}", assets_path, tilemap_sheet_path).c_str(), tile_map, is_front);
    }
}

void Game::LoadEntities(const json& map_data)
{
    for (auto& entities = map_data["Entities"]; const json& entity : entities)
    {
        std::string entity_name = entity["Name"];
        const int entity_id = entity["Id"];

        const auto game_entity = new Entity(component_manager, entity_pool);
        game_entity->SetName(entity_name.c_str());
        game_entity->SetId(entity_id);
        const auto entity_transform = game_entity->AddComponent<Transform>();
        entity_pool->AddEntity(game_entity);
        
        const bool is_active = entity["IsActive"];
        game_entity->SetIsActive(is_active);
        
        const bool is_persistent = entity["IsPersistent"];
        game_entity->SetIsPersistent(is_persistent);
        
        const auto& components = entity["Components"];
        for (const auto& component : components)
        {
            LoadComponents(component, game_entity, entity_transform);
        }
    }
}

void Game::LoadComponents(const json& component, Entity* game_entity, Transform* transform)
{
    const auto& component_type = component["Type"];
    if (component_type == "MapCollider")
    {
        int width = component["Width"];
        int height = component["Height"];
        game_entity->AddComponent<MapCollider>(width, height, map);
    }
    if (component_type == "BoxCollider")
    {
        int width = component["Width"];
        int height = component["Height"];
        game_entity->AddComponent<BoxCollider>(width, height, collision_solver);
    }
    if (component_type == "SpriteSheet")
    {
        int width = component["Width"];
        int height = component["Height"];
        std::string file_path = component["FilePath"];
        game_entity->AddComponent<SpriteSheet>(std::format("{}/{}", assets_path, file_path), width, height);
    }
    if (component_type == "Rigidbody")
    {
        const auto rigidbody = game_entity->AddComponent<Rigidbody>();
        const float mass = component["Mass"];
        const float drag = component["Drag"];
        rigidbody->SetMass(mass);
        rigidbody->SetDrag(drag);
    }
    if (component_type == "Transform")
    {
        const float x = component["X"];
        const float y = component["Y"];
        transform->SetPosition(x, y);
    }
    if (component_type == "Animator")
    {
        const auto animator = game_entity->AddComponent<Animator>();

        if (component.contains("Animation"))
        {
            const auto& animation = component["Animation"];
            const int row = animation["SpriteSheetRow"];
            const int start_frame = animation["StartFrame"];
            const int end_frame = animation["EndFrame"];
            const int frame_time = animation["FrameTime"];
            const bool is_loop = animation["IsLoop"];
            const bool play_on_setup = animation["PlayOnSetup"];
            animator->AddAnimation(row, start_frame, end_frame, frame_time, is_loop, play_on_setup);         
        }
    }
    if (component_type == "Agent")
    {
        const float speed = component["Speed"];
        const float stopping_distance = component["StoppingDistance"];
        const auto agent = game_entity->AddComponent<Agent>(map, speed, stopping_distance);
        
        const int target_entity_name = component["TargetEntityId"];
        const auto target_entity = entity_pool->GetEntityById(target_entity_name);
        const auto target_transform = target_entity->GetComponent<Transform>();
        agent->SetTarget(target_transform);
    }
    if (component_type == "PlayerController")
    {
        const float speed = component["Speed"];
        game_entity->AddComponent<PlayerController>(this, speed);
    }
    if (component_type == "CharacterAnimator")
    {
        auto animator = game_entity->GetComponent<Animator>();
        if (animator == nullptr)
        {
            animator = game_entity->AddComponent<Animator>();
        }
        
        const int move_anim_start_frame = component["MoveAnimationStartFrame"];
        const int move_anim_end_frame = component["MoveAnimationEndFrame"];
        const float move_anim_frame_time = component["MoveAnimationFrameTime"];
        
        const int idle_anim_start_frame = component["IdleAnimationStartFrame"];
        const int idle_anim_end_frame = component["IdleAnimationEndFrame"];
        const float idle_anim_frame_time = component["IdleAnimationFrameTime"];
        
        game_entity->AddComponent<CharacterAnimator>(animator,
            move_anim_start_frame, move_anim_end_frame, move_anim_frame_time,
            idle_anim_start_frame, idle_anim_end_frame, idle_anim_frame_time);
    }
    if (component_type == "MainCamera")
    {
        game_entity->AssignComponent<Camera>(Renderer::GetMainCamera());
    }
    if (component_type == "MapExit")
    {
        const std::string next_map_path = component["NextMapPath"];
        const float move_other_to_pos_x = component["MoveOtherToX"];
        const float move_other_to_pos_y = component["MoveOtherToY"];
        const Vector2* move_other_to_pos = new Vector2(move_other_to_pos_x, move_other_to_pos_y);
        game_entity->AddComponent<MapExit>(next_map_path.c_str(), move_other_to_pos, this);
    }
}

void Game::Setup() const
{
    component_manager->Setup();
}

void Game::Update(const float delta_time) const
{
    component_manager->Update(delta_time);
    collision_solver->Update();
}

void Game::Render() const
{
    SDL_RenderClear(Renderer::GetRenderer());
    
    map->RenderBackLayers();
    component_manager->Render();
    map->RenderFrontLayers();

#if _DEBUG    
    map->Debug_RenderPathNodes();
#endif
    
    SDL_RenderPresent(Renderer::GetRenderer());
}

void Game::Quit()
{
    is_running = false;
}

Game::~Game()
{
    is_running = false;
    Renderer::Destroy();
    SDL_Quit();
    delete map;
    delete component_manager;
    delete entity_pool;
}
