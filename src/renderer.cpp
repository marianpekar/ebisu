#include "renderer.h"

#include <iostream>
#include <SDL.h>
#include "ECS/Components/camera.h"
#include "Math/vector2.h"

SDL_Renderer* Renderer::s_renderer;
SDL_Window* Renderer::s_window;
Camera* Renderer::s_main_camera;
Vector2* Renderer::s_zero_vector = new Vector2(0,0);

bool Renderer::Initialize(const char* title, const int screen_width, const int screen_height, const bool fullscreen)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "[Renderer] Failed to initialize SDL subsystems" << std::endl;
        return false;
    }

    s_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
    if (!s_window)
    {
        std::cout << "[Renderer] Failed to create SDL Window" << std::endl;
        return false;
    }

    s_renderer = SDL_CreateRenderer(s_window, -1, 0);
    if (!s_window)
    {
        std::cout << "[Renderer] Failed to create SDL Renderer" << std::endl;
        return false;
    }

    SDL_SetRenderDrawColor(s_renderer, 0, 0, 0, 255);
    return true;
}

SDL_Renderer* Renderer::GetRenderer()
{
    return s_renderer;
}

void Renderer::Destroy()
{
    SDL_DestroyWindow(s_window);
    SDL_DestroyRenderer(s_renderer);
    delete s_zero_vector;
}

void Renderer::SetMainCamera(Camera* camera)
{
    s_main_camera = camera;
}

Camera* Renderer::GetMainCamera()
{
    return s_main_camera;
}

const Vector2* Renderer::TryGetCameraPosition()
{
    if(s_main_camera == nullptr)
        return s_zero_vector;

    return &s_main_camera->GetPosition();
}
