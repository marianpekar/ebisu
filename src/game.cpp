#include <fstream>
#include <iostream>
#include <string>
#include <SDL.h>
#include "game.h"
#include "map.h"
#include "transition_storage.h"
#include "renderer.h"
#include "texture_loader.h"
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
#include "ECS/Components/damage_receptor.h"
#include "ECS/Components/health.h"
#include "ECS/Components/map_exit.h"
#include "ECS/Components/projectile_emitter.h"
#include "ECS/Components/projectile_acceptor.h"

bool Game::Initialize(const std::string& title, const int screen_width, const int screen_height, const float scale_x, const float scale_y,
    const bool fullscreen, const std::string& map_path, const std::string& assets_root_path, const std::string& project_root_path)
{
    assets_path = std::format("{}/{}", project_root_path, assets_root_path);
    
    if (!Renderer::Initialize(title, screen_width, screen_height, scale_x, scale_x, fullscreen))
        return false;

    InitializeGameLogicEssentials(screen_width, screen_height);
    
    if (!LoadLevel(map_path))
    {
        std::cout << "[Game] Failed to load level" << '\n';
        return false;
    }

    component_manager->Setup();
    
    is_running = true;
    return is_running;
}

void Game::InitializeGameLogicEssentials(const int screen_width, const int screen_height)
{
    map = std::make_shared<Map>();
    component_manager = std::make_shared<ComponentManager>();
    collision_solver = std::make_shared<CollisionSolver>(0, 0, static_cast<float>(screen_width), static_cast<float>(screen_height));

    const std::shared_ptr<Camera> main_camera = std::make_shared<Camera>(screen_width, screen_height);
    Renderer::SetMainCamera(main_camera);
    entity_pool = std::make_shared<EntityPool>();
    transition_storage = std::make_shared<TransitionStorage>();
}

bool Game::LoadLevel(const std::string& map_path)
{
    std::ifstream map_file(std::format("{}/{}", assets_path, map_path));
    if (!map_file.is_open())
        return false;
    
    const json map_data = json::parse(map_file);
    LoadMap(map_data);
    LoadEntities(map_data);

    transition_storage->SetCurrentMapPath(map_path);
    entity_pool->LoadTransitionData(transition_storage);
    
    return true;
}

void Game::ChangeLevel(const std::string& next_map_path)
{
    transition_storage->SaveTransitionData(entity_pool);
    entity_pool->RemoveAllButPersistent();
    component_manager->RemoveAllButPersistent();
    component_manager->ResetComponents();
    collision_solver->Clear();
    map->Clear();
    TextureLoader::ClearTextureCache();
    LoadLevel(next_map_path);
    component_manager->Setup();
}

void Game::LoadMap(const json& map_data)
{
    const std::vector<int> collision_map = map_data["CollisionMap"];
    
    const int tile_size = map_data["TileSize"];
    const int rows = map_data["Rows"];
    const int cols = map_data["Columns"];
    const int map_width = tile_size * cols;
    const int map_height = tile_size * rows;
    
    map->SetDimensions(tile_size, map_width, map_height);
    map->SetCollisionMap(collision_map);
    map->GeneratePathNodes();
    
    const auto& layers = map_data["TileMapLayers"];
    for (const auto& layer : layers)
    {
        const std::string tilemap_sheet_path = layer["SpriteSheet"];
        const std::vector<int> tile_map = layer["TileMap"];
        const bool is_front = layer["IsFront"];
        map->AddLayer(std::format("{}/{}", assets_path, tilemap_sheet_path), tile_map, is_front);
    }
}

void Game::LoadEntities(const json& map_data)
{
    for (auto& entities = map_data["Entities"]; const json& entity : entities)
    {
        const int entity_id = entity["Id"];

        if (entity_pool->GetEntityById(entity_id) != nullptr)
            continue;
        
        std::string entity_name = entity["Name"];
        
        const auto game_entity = std::make_shared<Entity>(entity_id, entity_name, component_manager);
        
        const bool is_active = entity["IsActive"];
        game_entity->SetIsActive(is_active);
        
        const bool is_persistent = entity["IsPersistent"];
        game_entity->SetIsPersistent(is_persistent);

        entity_pool->AddEntity(game_entity);
        
        const auto& components = entity["Components"];
        for (const auto& component : components)
        {
            LoadComponents(component, game_entity);
        }
    }
}

void Game::LoadComponents(const json& component, const std::shared_ptr<Entity>& game_entity)
{
    const auto& component_type = component["Type"];
    if (component_type == "MapCollider")
    {
        float width = component["Width"];
        float height = component["Height"];
        float offset_x = component["OffsetX"];
        float offset_y = component["OffsetY"];
        game_entity->AddComponent<MapCollider>(width, height, offset_x, offset_y, map);
    }
    if (component_type == "BoxCollider")
    {
        float width = component["Width"];
        float height = component["Height"];
        float offset_x = component["OffsetX"];
        float offset_y = component["OffsetY"];
        bool is_trigger = component["IsTrigger"];
        game_entity->AddComponent<BoxCollider>(width, height, offset_x, offset_y, is_trigger, collision_solver);
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
        game_entity->AddComponent<Transform>(x, y);
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
        std::string next_map_path = component["NextMapPath"];
        const float move_other_to_pos_x = component["MoveOtherToX"];
        const float move_other_to_pos_y = component["MoveOtherToY"];
        game_entity->AddComponent<MapExit>(next_map_path, move_other_to_pos_x, move_other_to_pos_y, this);
    }
    if (component_type == "ProjectileEmitter")
    {
        std::string proj_sprite_sheet_filepath = component["ProjectileSpriteSheetFilePath"];
        const int proj_width = component["ProjectileWidth"];
        const int proj_height = component["ProjectileHeight"];
        const float proj_speed = component["ProjectileSpeed"];
        const uint32_t proj_life_time_ms = component["ProjectileLifeTime"];
        const size_t pool_size = component["PoolSize"];
        const uint32_t emit_delay_ms = component["EmitDelay"];
        const uint32_t animation_frame_time = component["ProjectileAnimationFrameTime"];
        const uint32_t animation_frames = component["ProjectileAnimationFrames"];
        game_entity->AddComponent<ProjectileEmitter>(std::format("{}/{}", assets_path, proj_sprite_sheet_filepath),
            proj_width, proj_height, proj_speed, proj_life_time_ms, pool_size, emit_delay_ms, map, collision_solver,
            animation_frame_time, animation_frames);
    }
    if (component_type == "ProjectileAcceptor")
    {
        game_entity->AddComponent<ProjectileAcceptor>();
    }
    if (component_type == "Health")
    {
        const float initial_health = component["InitialHealth"];
        const float max_health = component["MaxHealth"];
        game_entity->AddComponent<Health>(initial_health, max_health);
    }
    if (component_type == "DamageReceptor")
    {
        const float on_projectile_hit = component["OnProjectileHit"];
        game_entity->AddComponent<DamageReceptor>(on_projectile_hit);
    }
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
    TextureLoader::ClearTextureCache();
    Renderer::Destroy();
    SDL_Quit();
}
