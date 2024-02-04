#pragma once

#include <SDL.h>
#include <string>
#include <memory>
#include <stack>
#include <vector>
#include "../../Math/vector2.h"
#include "component.h"

struct Projectile
{
    Projectile()
    {
        dst_rect = new SDL_Rect();
    }
    Vector2 position;
    Vector2 direction;
    float speed = 0;
    uint32_t destroy_time = 0;
    SDL_Rect* dst_rect = nullptr;
    
    void Reset()
    {
        destroy_time = 0;
    }

    ~Projectile()
    {
        delete dst_rect;
    }
};

class ProjectilePool
{
    std::stack<std::shared_ptr<Projectile>> pool;
    
public:
    std::shared_ptr<Projectile> Get()
    {
        auto proj = pool.top();
        pool.pop();
        return proj;
    }
    
    void Return(const std::shared_ptr<Projectile>& proj)
    {
        proj->Reset();
        pool.push(proj);
    }

    void Add(const size_t count)
    {
        for(size_t i = 0; i < count; ++i)
        {
            pool.push(std::make_shared<Projectile>()); 
        }
    }

    bool IsEmpty() const
    {
        return pool.empty();
    }
};

class ProjectileEmitter : public Component
{
private:
    ProjectilePool projectile_pool;
    
    std::vector<std::shared_ptr<Projectile>> active_projectiles;
    uint32_t proj_life_time_ms = 0;
    float proj_speed = 0;
    size_t pool_size = 0;

    uint32_t last_emit_time = 0;
    uint32_t emit_delay_ms = 0;
    
    int proj_width = 0;
    int proj_height = 0;
    std::string proj_sprite_filepath;
    SDL_Texture* proj_sprite = nullptr;
    std::shared_ptr<class Transform> transform;
public:
    ProjectileEmitter(std::string proj_sprite_filepath, const int proj_width, const int proj_height, const float proj_speed, const uint32_t proj_life_time_ms, const size_t pool_size, const uint32_t emit_delay_ms)
        : proj_life_time_ms(proj_life_time_ms), proj_speed(proj_speed), pool_size(pool_size),
          emit_delay_ms(emit_delay_ms), proj_width(proj_width), proj_height(proj_height), proj_sprite_filepath(std::move(proj_sprite_filepath)) {}
    
    void Setup() override;
    void Update(float delta_time) override;
    void Render() override;
    void Reset() override;
    
    void Emit(const Vector2& direction);
};
