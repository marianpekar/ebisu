#pragma once

#include <SDL_rect.h>

#include "collider.h"
#include "../entity.h"
#include "../../Math/vector2.h"

class Projectile : public Collider
{
private:
    float width, height, half_width, half_height;
    Vector2 position, direction;
public:

    Projectile(int width, int height);
    
    float speed = 0;
    uint32_t destroy_time = 0;
    SDL_Rect* dst_rect = nullptr;
    SDL_Rect* src_rect = nullptr;

    void Reset()
    {
        is_active = false;
        destroy_time = 0;
    }

    ~Projectile() override
    {
        delete dst_rect;
        delete src_rect;
    }

    bool is_trigger = true;
    bool is_active = false;
    bool is_projectile = true;

    void SetPosition(const float x, const float y) { position.x = x, position.y = y; }
    void SetDirection(const float x, const float y) { direction.x = x, direction.y = y; }
    const Vector2& GetDirection() const { return direction; }
    
    const Vector2& GetPosition() const override { return position; }
    const float& GetX() const override { return position.x; }
    const float& GetY() const override { return position.y; }
    const float& GetWidth() const override { return width; }
    const float& GetHeight() const override { return height; }
    const float& GetHalfWidth() const override { return half_width; }
    const float& GetHalfHeight() const override { return half_height; }
    const bool& GetIsTrigger() const override { return is_trigger; }
    const std::shared_ptr<Entity>& GetOwner() override { return nullptr; }
    const std::shared_ptr<Rigidbody>& GetRigidbody() override { return nullptr; }

    void Collide(const std::shared_ptr<Collider>& other, const Vector2& overlap, bool one_is_trigger) override;
};

