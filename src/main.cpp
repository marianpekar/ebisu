#include "game.h"
#include <SDL.h>

int main(int argc, char*[])
{
    Game game;
    if (!game.Initialize("Ebisu", 1024, 768, false, "maps/map01.json", "assets"))
        return -1;

    game.Setup();

    constexpr Uint32 target_fps = 60;
    constexpr Uint32 max_frame_time = (1000 / target_fps);
    constexpr float max_delta_time = 0.05f;
    Uint32 ticks = 0;
    while (game.IsRunning())
    {
        while (SDL_GetTicks() < ticks + max_frame_time)
        {
            SDL_Delay(1);
        }

        float delta_time = static_cast<float>(SDL_GetTicks() - ticks) * 0.001f;

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
