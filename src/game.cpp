#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "game.h"
#include "ECS/entity_manager.h"
#include "ECS/entity.h"
#include "ECS/Components/sprite.h"

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
	Sprite* sprite = player->AddComponent<Sprite>();
	sprite->Setup("./../assets/test_square_64x64.png", renderer);
	
	Transform* transform = player->GetComponent<Transform>();
	transform->SetPosition(20,20);

	// TODO: Add controller component

	is_running = true;

	return 0;
}

void Game::HandleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
		case SDL_QUIT:
			is_running = false;
			break;
		default:
			break;
	}
}

void Game::Update()
{
	Uint32 current_time = SDL_GetTicks();
	float delta_time = (current_time - last_update_time) * 0.001f;
	entity_manager->Update(delta_time);
	last_update_time = current_time;
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

