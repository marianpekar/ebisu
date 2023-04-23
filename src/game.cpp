#include <iostream>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include "game.h"
#include "map.h"
#include "camera.h"
#include "ECS/component_manager.h"
#include "ECS/collision_solver.h"
#include "ECS/entity.h"
#include "ECS/Components/sprite_sheet.h"
#include "ECS/Components/player_controller.h"
#include "ECS/Components/transform.h"
#include "ECS/Components/animator.h"
#include "ECS/Components/map_collider.h"
#include "ECS/Components/box_collider.h"

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
	collision_solver = new CollisionSolver(0, 0, width, height);

	Entity* player = new Entity("Player", component_manager);

	Transform* transform = player->AddComponent<Transform>();
	camera = new Camera(transform, width, height);

	SpriteSheet* sprite = player->AddComponent<SpriteSheet>("./../assets/test_8dir_movement_animation_spritesheet_512x512.png", renderer, 64, 64, camera);
	Animator* animator = player->AddComponent<Animator>();

	player->AddComponent<BoxCollider>(64, 64, collision_solver);

	std::vector<int> tile_map {
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2,
		2, 0, 3, 1, 3, 1, 0, 2, 2, 0, 3, 1, 3, 1, 0, 2,
		2, 0, 1, 3, 1, 3, 0, 2, 2, 0, 1, 3, 1, 3, 0, 2,
		2, 0, 3, 1, 3, 1, 0, 2, 2, 0, 3, 1, 3, 1, 0, 2,
		2, 0, 1, 3, 1, 3, 0, 2, 2, 0, 1, 3, 1, 3, 0, 2,
		2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2,
		2, 0, 3, 1, 3, 1, 0, 2, 2, 0, 3, 1, 3, 1, 0, 2,
		2, 0, 1, 3, 1, 3, 0, 2, 2, 0, 1, 3, 1, 3, 0, 2,
		2, 0, 3, 1, 3, 1, 0, 2, 2, 0, 3, 1, 3, 1, 0, 2,
		2, 0, 1, 3, 1, 3, 0, 2, 2, 0, 1, 3, 1, 3, 0, 2,
		2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	};

	std::vector<int> collision_map {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0,
		0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0,
		0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0,
		0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0,
		0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0,
		0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 
		0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	map = new Map(renderer, 64, 1024, camera, collision_map);
	map->AddLayer("./../assets/test_tilemap_4_tiles_128x128.png", tile_map);

	player->AddComponent<MapCollider>(64, 64, map);
	player->AddComponent<PlayerController>(is_running);

	Entity* static_animated = new Entity("StaticAnimated", component_manager);
	Transform* sa_transform = static_animated->AddComponent<Transform>();
	sa_transform->Move(96, 96);
	SpriteSheet* sa_sheet = static_animated->AddComponent<SpriteSheet>("./../assets/test_4_frames_transparent_spritesheet_64x256.png", renderer, 64, 64, camera);
	Animator* sa_animator = static_animated->AddComponent<Animator>();
	static_animated->AddComponent<BoxCollider>(64, 64, collision_solver);
	sa_animator->AddAnimation(0, 0, 3, 500, true, true);


	for (size_t i = 0; i < 20; i++)
	{
		Entity* static_animated_2 = new Entity("StaticAnimated", component_manager);
		Transform* sa_transform_2 = static_animated_2->AddComponent<Transform>();
		sa_transform_2->Move(rand() % 1000, rand() % 1000);
		SpriteSheet* sa_sheet_2 = static_animated_2->AddComponent<SpriteSheet>("./../assets/test_4_frames_transparent_spritesheet_64x256.png", renderer, 64, 64, camera);
		Animator* sa_animator_2 = static_animated_2->AddComponent<Animator>();
		static_animated_2->AddComponent<BoxCollider>(64, 64, collision_solver);
		sa_animator_2->AddAnimation(0, 0, 3, 500, true, true);
	}

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
	collision_solver->Update();
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

