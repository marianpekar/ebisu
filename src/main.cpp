#include "game.h"
#include <SDL.h>

#include "config_map.h"

const std::string project_path =
#if _DEBUG
    "./..";
#else
    ".";
#endif

int main(int argc, char*[])
{
    const std::string game_ini_path = project_path + "/config/game.ini";
    ConfigMap* config_map = new ConfigMap(game_ini_path.c_str());
    const std::string title = config_map->GetString("Title");
    const int screen_width = config_map->GetInt("ScreenWidth");
    const int screen_height = config_map->GetInt("ScreenHeight");
    const bool is_fullscreen = config_map->GetBool("Fullscreen");
    const std::string initial_map_path = config_map->GetString("MapPath");
    const std::string assets_path = config_map->GetString("AssetsRootPath");
    
    Game game;
    if (!game.Initialize(title.c_str(), screen_width, screen_height, is_fullscreen, initial_map_path.c_str(), assets_path.c_str(), project_path.c_str()))
        return -1;

    delete config_map;
    
    game.Setup();

    constexpr Uint32 target_fps = 60;
    constexpr Uint32 frame_time = 1000 / target_fps;
    constexpr float fixed_delta_time = 1.0f / target_fps;

    Uint32 previous_ticks = SDL_GetTicks();
    float accumulated_time = 0.0f;

    while (game.IsRunning())
    {
        const Uint32 current_ticks = SDL_GetTicks();
        const Uint32 frame_ticks = current_ticks - previous_ticks;

        if (frame_ticks < frame_time)
        {
            SDL_Delay(frame_time - frame_ticks);
            continue;
        }

        accumulated_time += 0.001f * frame_ticks;

        while (accumulated_time >= fixed_delta_time)
        {
            game.Update(fixed_delta_time);
            accumulated_time -= fixed_delta_time;
        }

        game.Render();

        previous_ticks = current_ticks;
    }



    return 0;
}
