#pragma once

#include <string>
#include "../../Math/vector2.h"

#include "component.h"

class SpriteSheet final : public Component
{
private:
    int sheet_width, sheet_height, rect_width, rect_height;
    struct SDL_Texture* sprite = nullptr;
    struct SDL_Renderer* renderer;
    struct SDL_Rect* src_rect = nullptr;
    SDL_Rect* dst_rect = nullptr;
    std::string filepath;
    class Transform* transform = nullptr;
    class Camera* camera;
    const Vector2 zero_vector = Vector2(0.f,0.f);
    const Vector2& TryGetCameraPosition() const;
public:
    SpriteSheet(std::string filepath, SDL_Renderer* renderer, int rect_width, int rect_height);
    ~SpriteSheet() override;
    void Setup() override;
    void Render() override;
    void SelectSprite(const int& row, const int& col) const;
    void SetCamera(Camera* new_camera) { camera = new_camera; }
};