#include "renderer.h"

#include <iostream>
#include <SDL.h>

#include "ECS/Components/camera.h"
#include "Math/vector2.h"

SDL_Renderer* Renderer::s_renderer;
SDL_Window* Renderer::s_window;
std::shared_ptr<Camera> Renderer::s_main_camera;

bool Renderer::Initialize(const std::string& title, const int screen_width, const int screen_height, const float scale_x, const float scale_y, const bool fullscreen)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "[Renderer] Failed to initialize SDL subsystems" << '\n';
        return false;
    }

    s_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
    if (!s_window)
    {
        std::cout << "[Renderer] Failed to create SDL Window" << '\n';
        return false;
    }

    s_renderer = SDL_CreateRenderer(s_window, -1, 0);
    if (!s_window)
    {
        std::cout << "[Renderer] Failed to create SDL Renderer" << '\n';
        return false;
    }

    SDL_RenderSetScale(s_renderer, scale_x, scale_y);

    SDL_SetRenderDrawColor(s_renderer, 0, 0, 0, 255);
    return true;
}

void Renderer::GetScale(float& scale_x, float& scale_y)
{
    SDL_RenderGetScale(s_renderer, &scale_x, &scale_y);
}

SDL_Renderer* Renderer::GetRenderer()
{
    return s_renderer;
}

void Renderer::Destroy()
{
    SDL_DestroyWindow(s_window);
    SDL_DestroyRenderer(s_renderer);
}

void Renderer::SetMainCamera(const std::shared_ptr<Camera>& camera)
{
    s_main_camera = camera;
}

std::shared_ptr<Camera> Renderer::GetMainCamera()
{
    return s_main_camera;
}

const Vector2& Renderer::TryGetCameraPosition()
{
    if(s_main_camera == nullptr)
        return Vector2::Zero();

    float scale_x, scale_y;
    SDL_RenderGetScale(s_renderer, &scale_x, &scale_y);

    static Vector2 adjusted_position;
    adjusted_position = s_main_camera->GetPosition();
    adjusted_position.x /= scale_x;
    adjusted_position.y /= scale_y;

    return adjusted_position;
}

void Renderer::DrawLine(const Vector2& a, const Vector2& b)
{
    const auto camera_pos = TryGetCameraPosition();
    SDL_RenderDrawLine(s_renderer,
        static_cast<int>(a.x - camera_pos.x),
        static_cast<int>(a.y - camera_pos.y),
        static_cast<int>(b.x - camera_pos.x),
        static_cast<int>(b.y - camera_pos.y));
}


