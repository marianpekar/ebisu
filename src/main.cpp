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
    auto config_map = std::make_shared<ConfigMap>(game_ini_path);
    const std::string title = config_map->GetString("Title");
    const int screen_width = config_map->GetInt("ScreenWidth");
    const int screen_height = config_map->GetInt("ScreenHeight");
    const bool is_fullscreen = config_map->GetBool("Fullscreen");
    const float scale_x = config_map->GetFloat("ScaleX");
    const float scale_y = config_map->GetFloat("ScaleY");
    const std::string initial_map_path = config_map->GetString("MapPath");
    const std::string assets_path = config_map->GetString("AssetsRootPath");
    
    Game game;
    if (!game.Initialize(title, screen_width, screen_height, scale_x, scale_y, is_fullscreen, initial_map_path, assets_path, project_path))
        return -1;
    
    config_map.reset();
    
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

        const float delta_time = 0.001f * static_cast<float>(frame_ticks);
        game.Update(delta_time);

        accumulated_time += delta_time;

        while (accumulated_time >= fixed_delta_time)
        {
            game.FixedUpdate();
            accumulated_time -= fixed_delta_time;
        }

        game.Render();

        previous_ticks = current_ticks;
    }
    
    return 0;
}
