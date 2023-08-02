#pragma once

#include "json.hpp"

using json = nlohmann::json;

class Game 
{
public:
	Game() = default;
	~Game();
	int Initialize(const char* title, int width, int height, bool fullscreen);
	void Setup();
	void Update(float delta_time);
	void Render();
	const bool& IsRunning() { return is_running; }
private:
	bool LoadMap(int width, int height);
	void LoadTilemaps(json& map_data, class Transform* transform, int width, int height);
	bool is_running = false;
	class ComponentManager* component_manager = nullptr;
	class CollisionSolver* collision_solver = nullptr;
	class Map* map = nullptr;
	class Camera* camera = nullptr;
	struct SDL_Window* window = nullptr;
	struct SDL_Renderer* renderer = nullptr;
};