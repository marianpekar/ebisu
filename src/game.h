#pragma once

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
	bool is_running = false;
	class ComponentManager* component_manager = nullptr;
	class Map* map = nullptr;
	struct SDL_Window* window = nullptr;
	struct SDL_Renderer* renderer = nullptr;
};