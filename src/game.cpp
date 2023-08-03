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
		
	json map_data = nlohmann::json::parse(map_file);

	Entity* player = new Entity("Player", component_manager);
	Transform* player_transform = player->AddComponent<Transform>();

	LoadTilemaps(map_data, player_transform, width, height);
	
	auto& entities = map_data["Entities"];

	for (const json& entity : entities) 
	{
		std::string entity_name = entity["Name"];

		if (entity_name == "Player")
		{
			LoadPlayer(entity, player, player_transform);
			continue;
		}

		Entity* game_entity = new Entity(entity_name.c_str(), component_manager);
		Transform* entity_transform = game_entity->AddComponent<Transform>();
		LoadEntity(entity, game_entity, entity_transform);
	}

	return true;
}

void Game::LoadTilemaps(const json& map_data, Transform* transform, int width, int height)
{
	std::vector<int> tile_map = map_data["Tilemap"];
	std::vector<int> collision_map = map_data["CollisionMap"];

	camera = new Camera(transform, width, height);

	std::string tilemap_sheet_path = map_data["TilemapSpriteSheet"];

	int tile_size = map_data["TileSize"];
	int row_tile_count = map_data["RowTileCount"];
	int map_size = tile_size * row_tile_count;

	map = new Map(renderer, tile_size, map_size, camera, collision_map);
	map->AddLayer(tilemap_sheet_path.c_str(), tile_map);
}

void Game::LoadPlayer(const json& entity, Entity* player, Transform* transform)
{
	const auto& components = entity["Components"];
	for (const auto& component : components)
	{
		LoadComponents(component, player, transform);
	}

	player->AddComponent<Animator>();
	player->AddComponent<PlayerController>(is_running);
}

void Game::LoadEntity(const json& entity, Entity* game_entity, Transform* transform)
{
	const auto& components = entity["Components"];
	for (const auto& component : components)
	{
		LoadComponents(component, game_entity, transform);
	}
}

void Game::LoadComponents(const json& component, Entity* game_entity, Transform* transform)
{
	auto component_type = component["Type"];
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
		game_entity->AddComponent<SpriteSheet>(file_path, renderer, width, height, camera);
	}
	if (component_type == "Rigidbody")
	{
		Rigidbody* rigidbody = game_entity->AddComponent<Rigidbody>();
		float mass = component["Mass"];
		float drag = component["Drag"];
		rigidbody->SetMass(mass);
		rigidbody->SetDrag(drag);
	}
	if (component_type == "Transform")
	{
		float x = component["X"];
		float y = component["Y"];
		transform->SetPosition(x, y);
	}
	if (component_type == "Animator")
	{
		Animator* animator = game_entity->AddComponent<Animator>();
		const auto& animation = component["Animation"];
		int row = animation["SpriteSheetRow"];
		int start_frame = animation["StartFrame"];
		int end_frame = animation["EndFrame"];
		int frame_time = animation["FrameTime"];
		bool is_loop = animation["IsLoop"];
		bool play_on_setup = animation["PlayOnSetup"];
		animator->AddAnimation(row, start_frame, end_frame, frame_time, is_loop, play_on_setup);
	}
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

