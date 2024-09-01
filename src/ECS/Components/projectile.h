#pragma once

#include <SDL_rect.h>

#include "collider.h"
#include "../entity.h"
#include "../../Math/vector2.h"

class Projectile : public Collider
{
private:
    Vector2 direction;

    int current_frame = 0;
    int frames;

    float speed = 0;
    uint32_t destroy_time = 0;

    bool is_active = false;
public:
    SDL_Rect* dst_rect = nullptr;
    SDL_Rect* src_rect = nullptr;
    
    Projectile(int width_val, int height_val, int frames);
    ~Projectile() override;

    void NextFrame();
private:
    void SelectSpriteCol(const int& col) const;
public:
    void SelectSpriteRow(const int& row) const;
    
    void Reset();

    const Vector2& GetDirection() const { return direction; }
    void SetDirection(const float x, const float y) { direction.x = x; direction.y = y; }

    const float& GetSpeed() const { return speed; }
    void SetSpeed(const float new_speed) { speed = new_speed; }
    
    const bool& GetIsActive() const { return is_active; }
    void SetIsActive(const bool active) { is_active = active; } 
    
    const uint32_t& GetDestroyTime() const { return destroy_time; }
    void SetDestroyTime(const uint32_t new_destroy_time) { destroy_time = new_destroy_time; }

    const std::shared_ptr<Entity>& GetOwner() override { return nullptr; }
    const std::shared_ptr<Rigidbody>& GetRigidbody() override { return nullptr; }
    
    void Collide(const std::shared_ptr<Collider>& other, const Vector2& overlap, bool one_is_trigger) override;
};