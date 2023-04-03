#pragma once

class Game 
{
public:
	Game() = default;
	~Game();
	int Initialize(const char* title, int width, int height, bool fullscreen);
	void HandleEvents();
	void Update();
	void Render();
	const bool& IsRunning() { return is_running; }
private:
	bool is_running = false;
	int last_update_time = 0;
	class EntityManager* entity_manager;
	struct SDL_Window* window = nullptr;
	struct SDL_Renderer* renderer = nullptr;
};