#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "game.h"
#include "ECS/entity_manager.h"
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
	
	this->entity_manager = new EntityManager();

	Entity* player = new Entity("Player", entity_manager);
	Transform* transform = player->AddComponent<Transform>();
	SpriteSheet* sprite = player->AddComponent<SpriteSheet>("./../assets/test_8dir_movement_animation_spritesheet_512x512.png", renderer, 64, 64);
	Animator* animator = player->AddComponent<Animator>();

	player->AddComponent<PlayerController>(is_running);

	is_running = true;

	return 0;
}

void Game::Setup()
{
	entity_manager->Setup();
}

void Game::Update(float delta_time)
{
	entity_manager->Update(delta_time);
}

void Game::Render()
{
	SDL_RenderClear(renderer);
	entity_manager->Render();
	SDL_RenderPresent(renderer);
}

Game::~Game()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	delete entity_manager;
}

