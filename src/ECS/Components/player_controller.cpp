#include "player_controller.h"
#include "transform.h"
#include <SDL.h>

void PlayerController::Update(float delta_time)
{
	SDL_Event event;
	SDL_PollEvent(&event);
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	switch (event.type)
	{
		case SDL_QUIT:
			is_game_running = false;
			break;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			if (keys[SDL_SCANCODE_W])
			{
				y = -move_speed * delta_time;
			}
			if (keys[SDL_SCANCODE_S])
			{
				y = move_speed * delta_time;
			}
			if (keys[SDL_SCANCODE_A])
			{
				x = -move_speed * delta_time;
			}
			if (keys[SDL_SCANCODE_D])
			{
				x = move_speed * delta_time;
			}

			if (x == 1 && y == 1)
			{
				x = y = 0.5;
			}

			if (x == -1 && y == -1)
			{
				x = y = -0.5;
			}

			//TODO: This is temporary, once you'll have physics, use forces for movement
			transform->Move(x, y);

			x = y = 0;
			
			break;
		default:
			break;
	}
}
