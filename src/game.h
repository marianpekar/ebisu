#pragma once

#include "json.hpp"

using json = nlohmann::json;

class Game 
{
public:
    Game() = default;
    ~Game();
    bool Initialize(const char* title, int screen_width, int screen_height, bool fullscreen);
    void InitializeGameLogicEssentials(int screen_width, int screen_height);
    void Setup() const;
    void Update(float delta_time) const;
    void Render() const;
    const bool& IsRunning() const { return is_running; }
    void Quit();

private:
    bool LoadLevel();
    void LoadMap(const json& map_data);
    void LoadEntities(const json& map_data);
    void LoadComponents(const json& component, class Entity* game_entity, class Transform* transform);
    bool is_running = false;
    class ComponentManager* component_manager = nullptr;
    class CollisionSolver* collision_solver = nullptr;
    class Map* map = nullptr;
    class EntityPool* entity_pool;
    class Camera* main_camera;
};