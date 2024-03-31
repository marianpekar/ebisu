#pragma once

#include <SDL_rect.h>

#include "collider.h"
#include "../entity.h"
#include "../../Math/vector2.h"

class Projectile : public Collider
{
private:
    int width, height, half_width, half_height;
    Vector2 position, direction;

    int current_frame = 0;
    int frames;

    float speed = 0;
    uint32_t destroy_time = 0;

    bool is_trigger = true;
    bool is_active = false;
public:
    SDL_Rect* dst_rect = nullptr;
    SDL_Rect* src_rect = nullptr;
    
    Projectile(int width, int height, int frames);
    ~Projectile() override;

    void NextFrame();
private:
    void SelectSpriteCol(const int& col) const;
public:
    void SelectSpriteRow(const int& row) const;
    
    void Reset();
    
    void SetPosition(const float x, const float y) { position.x = x, position.y = y; }
    void SetDirection(const float x, const float y) { direction.x = x, direction.y = y; }
    const Vector2& GetDirection() const { return direction; }
    
    const float& GetSpeed() const { return speed; }
    void SetSpeed(const float new_speed) { speed = new_speed; }
    
    const bool& GetIsActive() const { return is_active; }
    void SetIsActive(const bool active) { is_active = active; } 
    
    const uint32_t& GetDestroyTime() const { return destroy_time; }
    void SetDestroyTime(const uint32_t new_destroy_time) { destroy_time = new_destroy_time; }
    
    // Collider
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

