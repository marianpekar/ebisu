#pragma once

class Renderer
{
    static struct SDL_Renderer* s_renderer;
    static struct SDL_Window* s_window;
public:
    static bool Initialize(const char* title, const int screen_width, const int screen_height, const bool fullscreen);
    static SDL_Renderer* GetRenderer();
    static void Destroy();
};
