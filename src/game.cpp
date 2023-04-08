#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "game.h"
#include "map.h"
#include "camera.h"
#include "ECS/component_manager.h"
#include "ECS/entity.h"
#include "ECS/Components/sprite_sheet.h"
#include "ECS/Components/player_controller.h"
#include "ECS/Components/transform.h"
#include "ECS/Components/animator.h"

int Game::Initialize(const char* title, int width, int height, bool fullscreen)
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

	Entity* player = new Entity("Player", component_manager);

	Transform* transform = player->AddComponent<Transform>();
	camera = new Camera(transform, width, height);

	SpriteSheet* sprite = player->AddComponent<SpriteSheet>("./../assets/test_8dir_movement_animation_spritesheet_512x512.png", renderer, 64, 64, camera);
	Animator* animator = player->AddComponent<Animator>();

	player->AddComponent<PlayerController>(is_running);

	std::vector<int> tilemap{
		2, 2, 2, 2, 2, 2, 2, 2,
		2, 0, 0, 0, 0, 0, 0, 2,
		2, 0, 3, 1, 3, 1, 0, 2,
		2, 0, 1, 3, 1, 3, 0, 2,
		2, 0, 3, 1, 3, 1, 0, 2,
		2, 0, 1, 3, 1, 3, 0, 2,
		2, 0, 0, 0, 0, 0, 0, 2,
		2, 2, 2, 2, 2, 2, 2, 2,
	};

	map = new Map(renderer, 64, 512, camera);
	map->AddLayer("./../assets/test_tilemap_4_tiles_256x64.png", tilemap);

	is_running = true;

	return 0;
}

void Game::Setup()
{
	component_manager->Setup();
}

void Game::Update(float delta_time)
{
	component_manager->Update(delta_time);
	camera->Update();
}

void Game::Render()
{
	SDL_RenderClear(renderer);
	map->Render();
	component_manager->Render();
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
}

