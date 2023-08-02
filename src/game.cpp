#include <iostream>
#include <fstream>
#include <string>
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
#include "ECS/Components/rigidbody.h"
#include "json.hpp"

using json = nlohmann::json;

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

	bool success = LoadMap(width, height);
	if (!success)
	{
		std::cout << "[Game] Failed to load map" << std::endl;
		return -1;
	}

	is_running = true;

	return 0;
}

bool Game::LoadMap(int width, int height)
{
	std::ifstream map_file("./../assets/maps/map01.json");
	if (!map_file.is_open())
		return false;
		
	json map_data = json::parse(map_file);

	auto tile_map_array = map_data["Tilemap"];
	std::vector<int> tile_map = tile_map_array.get<std::vector<int>>();

	auto collision_map_array = map_data["CollisionMap"];
	std::vector<int> collision_map = collision_map_array.get<std::vector<int>>();

	Entity* player = new Entity("Player", component_manager);

	Transform* transform = player->AddComponent<Transform>();

	camera = new Camera(transform, width, height);

	std::string tilemap_sheet_path = map_data["TilemapSpriteSheet"].get<std::string>();

	map = new Map(renderer, 64, 1024, camera, collision_map);
	map->AddLayer(tilemap_sheet_path.c_str(), tile_map);

	SpriteSheet* sprite = player->AddComponent<SpriteSheet>("./../assets/test_8dir_movement_animation_spritesheet_512x512.png", renderer, 64, 64, camera);
	Animator* animator = player->AddComponent<Animator>();

	player->AddComponent<BoxCollider>(64, 64, collision_solver);

	player->AddComponent<MapCollider>(64, 64, map);
	player->AddComponent<PlayerController>(is_running);
	player->AddComponent<Rigidbody>();

	for (size_t i = 0; i < 30; i++)
	{
		Entity* other_entity = new Entity("Other Entity", component_manager);
		Transform* other_entity_transform = other_entity->AddComponent<Transform>();
		other_entity_transform->Move(rand() % 1000, rand() % 1000);
		SpriteSheet* oe_sheet = other_entity->AddComponent<SpriteSheet>("./../assets/test_4_frames_transparent_spritesheet_64x256.png", renderer, 64, 64, camera);
		Animator* oe_animator = other_entity->AddComponent<Animator>();
		other_entity->AddComponent<BoxCollider>(64, 64, collision_solver);
		oe_animator->AddAnimation(0, 0, 3, 500, true, true);
		other_entity->AddComponent<MapCollider>(64, 64, map);
		Rigidbody* rb = other_entity->AddComponent<Rigidbody>();
		rb->SetMass(10);
		rb->SetDrag(0.5);
	}

	return true;
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

