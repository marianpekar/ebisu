#include "game.h"
#include <fstream>
#include <iostream>
#include <SDL.h>
#include <string>
#include "camera.h"
#include "map.h"
#include "ECS/collision_solver.h"
#include "ECS/component_manager.h"
#include "ECS/entity.h"
#include "ECS/entity_pool.h"
#include "ECS/Components/agent.h"
#include "ECS/Components/animator.h"
#include "ECS/Components/box_collider.h"
#include "ECS/Components/map_collider.h"
#include "ECS/Components/player_controller.h"
#include "ECS/Components/rigidbody.h"
#include "ECS/Components/sprite_sheet.h"
#include "ECS/Components/transform.h"

const std::string project_path =
#if _DEBUG
    "./../";
#else
    "./"
#endif

const std::string assets_path = project_path + "assets";

int Game::Initialize(const char* title, const int width, const int height, const bool fullscreen)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "[Game] Failed to initialize SDL subsystems" << std::endl;
        return -1;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
    if (!window)
    {
        std::cout << "[Game] Failed to create SDL Window" << std::endl;
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!window)
    {
        std::cout << "[Game] Failed to create SDL Renderer" << std::endl;
        return -1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    component_manager = new ComponentManager();
    collision_solver = new CollisionSolver(0, 0, static_cast<float>(width), static_cast<float>(height));

    entity_pool = new EntityPool();
    
    if (const bool success = LoadMap(width, height); !success)
    {
        std::cout << "[Game] Failed to load map" << std::endl;
        return -1;
    }

    is_running = true;

    return 0;
}

void Game::Quit()
{
    is_running = false;
}

bool Game::LoadMap(const int width, const int height)
{
    std::ifstream map_file("./../assets/maps/map01.json");
    if (!map_file.is_open())
        return false;

    json map_data = nlohmann::json::parse(map_file);
    
    const auto player = new Entity(component_manager, entity_pool);
    const auto player_transform = player->AddComponent<Transform>();

    LoadTilemaps(map_data, player_transform, width, height);

    for (auto& entities = map_data["Entities"]; const json& entity : entities)
    {
        std::string entity_name = entity["Name"];
        const int entity_id = entity["Id"];
        
        if (entity_name == "Player")
        {
            player->SetName(entity_name.c_str());
            player->SetId(entity_id);
            LoadPlayer(entity, player, player_transform);
            continue;
        }

        const auto game_entity = new Entity(component_manager, entity_pool);
        game_entity->SetName(entity_name.c_str());
        game_entity->SetId(entity_id);
        const auto entity_transform = game_entity->AddComponent<Transform>();
        entity_pool->AddEntity(game_entity);
        LoadEntity(entity, game_entity, entity_transform);
    }
    
    return true;
}

void Game::LoadTilemaps(const json& map_data, Transform* transform, const int width, const int height)
{
    const std::vector<int> collision_map = map_data["CollisionMap"];

    camera = new Camera(transform, width, height);

    const int tile_size = map_data["TileSize"];
    const int rows = map_data["Rows"];
    const int cols = map_data["Columns"];
    const int map_width = tile_size * cols;
    const int map_height = tile_size * rows;

    map = new Map(renderer, tile_size, map_width, map_height, camera, collision_map);

    const auto& layers = map_data["TileMapLayers"];
    for (const auto& layer : layers)
    {
        const std::string tilemap_sheet_path = layer["SpriteSheet"];
        const std::vector<int> tile_map = layer["TileMap"];
        const bool is_front = layer["IsFront"];
        map->AddLayer(std::format("{}/{}", assets_path, tilemap_sheet_path).c_str(), tile_map, is_front);
    }
}

void Game::LoadPlayer(const json& entity, Entity* player, Transform* transform)
{
    const bool is_active = entity["IsActive"];
    player->SetIsActive(is_active);

    const auto& components = entity["Components"];
    for (const auto& component : components)
    {
        LoadComponents(component, player, transform);
    }

    player->AddComponent<Animator>();
    player->AddComponent<PlayerController>(this);
}

void Game::LoadEntity(const json& entity, Entity* game_entity, Transform* transform)
{
    const bool is_active = entity["IsActive"];
    game_entity->SetIsActive(is_active);

    const auto& components = entity["Components"];
    for (const auto& component : components)
    {
        LoadComponents(component, game_entity, transform);
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
        game_entity->AddComponent<SpriteSheet>(std::format("{}/{}", assets_path, file_path), renderer, width, height, camera);
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
        const auto& animation = component["Animation"];
        const int row = animation["SpriteSheetRow"];
        const int start_frame = animation["StartFrame"];
        const int end_frame = animation["EndFrame"];
        const int frame_time = animation["FrameTime"];
        const bool is_loop = animation["IsLoop"];
        const bool play_on_setup = animation["PlayOnSetup"];
        animator->AddAnimation(row, start_frame, end_frame, frame_time, is_loop, play_on_setup);
    }
    if (component_type == "Agent")
    {
        const auto agent = game_entity->AddComponent<Agent>(map);
        const int target_entity_name = component["TargetEntityId"];
        const auto target_entity = entity_pool->GetEntityById(target_entity_name);
        const auto target_transform = target_entity->GetComponent<Transform>();
        agent->SetTarget(target_transform);
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
    camera->Update();
}

void Game::Render() const
{
    SDL_RenderClear(renderer);
    map->RenderBackLayers();
    component_manager->Render();
    map->RenderFrontLayers();
    
    map->Debug_RenderPathNodes();
    
    SDL_RenderPresent(renderer);
}

Game::~Game()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    delete map;
    delete camera;
    delete component_manager;
    delete entity_pool;
}
