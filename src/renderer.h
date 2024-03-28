#pragma once
#include <memory>
#include <string>

class Renderer
{
    static struct SDL_Renderer* s_renderer;
    static struct SDL_Window* s_window;
    static std::shared_ptr<class Camera> s_main_camera;
public:
    static bool Initialize(const std::string& title, int screen_width, int screen_height, float scale_x, float scale_y, bool fullscreen);
    static void GetScale(float& scale_x, float& scale_y);
    static SDL_Renderer* GetRenderer();
    static void Destroy();
    static void SetMainCamera(const std::shared_ptr<Camera>& camera);
    static std::shared_ptr<Camera> GetMainCamera();
    static const struct Vector2& TryGetCameraPosition();
    static void DrawLine(const Vector2& a, const Vector2& b);
};
