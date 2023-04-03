#include "game.h"
#include "ECS/entity_manager.h"
#include "ECS/entity.h"
#include "ECS/Components/transform.h"
#include "ECS/Components/sprite.h"

int main(int argc, char* args[])
{
	Game game;
	if (game.Initialize("Ebisu", 1024, 768, false) != 0)
		return -1;

	while (game.IsRunning())
	{
		game.HandleEvents();
		game.Update();
		game.Render();
	}

	return 0;
}
