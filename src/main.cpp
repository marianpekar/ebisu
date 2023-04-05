#include "game.h"
#include "ECS/entity_manager.h"
#include "ECS/entity.h"
#include "ECS/Components/transform.h"
#include "ECS/Components/sprite.h"
#include <SDL.h>

int main(int argc, char* args[])
{
	Game game;
	if (game.Initialize("Ebisu", 1024, 768, false) != 0)
		return -1;

	game.Setup();

	const Uint32 target_fps = 60;
	const Uint32 max_frame_time = (1000 / target_fps);
	const float max_delta_time = 0.05f;
	Uint32 ticks = 0;
	while (game.IsRunning())
	{
		while (SDL_GetTicks() < ticks + max_frame_time)
		{
			SDL_Delay(1);
		}

		float delta_time = (SDL_GetTicks() - ticks) * 0.001f;

		if (delta_time > max_delta_time)
		{
			delta_time = max_delta_time;
		}

		game.Update(delta_time);
		game.Render();
		
		ticks = SDL_GetTicks();
	}

	return 0;
}
