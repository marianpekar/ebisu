#pragma once

#include <memory>
#include "json.hpp"

using json = nlohmann::json;

class Game 
{
public:
    Game() = default;
    ~Game();
    bool Initialize(const std::string& title, int screen_width, int screen_height, bool fullscreen, const std::string& map_path, const std::string& assets_root_path, const std::string& project_root_path);
    void InitializeGameLogicEssentials(int screen_width, int screen_height);
    void Setup() const;
    void Update(float delta_time) const;
    void Render() const;
    const bool& IsRunning() const { return is_running; }
    void Quit();
    bool LoadLevel(const std::string& map_path);
    void ChangeLevel(const std::string& string);

private:
    std::string assets_path;
    void LoadMap(const json& map_data);
    void LoadEntities(const json& map_data);
    void LoadComponents(const json& component, const std::shared_ptr<class Entity>& game_entity);
    bool is_running = false;
    std::shared_ptr<class ComponentManager> component_manager = nullptr;
    std::shared_ptr<class CollisionSolver> collision_solver = nullptr;
    std::shared_ptr<class Map> map = nullptr;
    std::shared_ptr<class EntityPool> entity_pool = nullptr;
};