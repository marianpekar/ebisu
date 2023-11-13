#pragma once

class Renderer
{
    static struct SDL_Renderer* s_renderer;
    static struct SDL_Window* s_window;
    static class Camera* s_main_camera;
public:
    static bool Initialize(const char* title, int screen_width, int screen_height, bool fullscreen);
    static SDL_Renderer* GetRenderer();
    static void Destroy();
    static void SetMainCamera(Camera* camera);
    static Camera* GetMainCamera();
    static const struct Vector2& TryGetCameraPosition();
    static void DrawLine(const Vector2& a, const Vector2& b);
};
