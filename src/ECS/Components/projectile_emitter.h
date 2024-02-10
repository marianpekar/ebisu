#pragma once

#include <SDL.h>
#include <string>
#include <memory>
#include <stack>
#include <vector>

#include "component.h"
#include "collider.h"
#include "player_controller.h"
#include "../collision_solver.h"
#include "../entity.h"
#include "../../Math/vector2.h"

class Projectile : public Collider
{
private:
    float width, height, half_width, half_height;
    Vector2 position, direction;
public:

    Projectile(const int width, const int height)
        : width(static_cast<float>(width)), height(static_cast<float>(height)),
          half_width(static_cast<float>(width) / 2.f), half_height(static_cast<float>(height) / 2.f)
    {
        dst_rect = new SDL_Rect();

        src_rect = new SDL_Rect();
        src_rect->x = 0;
        src_rect->y = 0;
        src_rect->w = width;
        src_rect->h = height;
    }


    float speed = 0;
    uint32_t destroy_time = 0;
    SDL_Rect* dst_rect = nullptr;
    SDL_Rect* src_rect = nullptr;

    void Reset()
    {
        destroy_time = 0;
    }

    ~Projectile()
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

    void Collide(const std::shared_ptr<Collider>& other, const Vector2& overlap, bool one_is_trigger) override
    {
        if (!is_active || (other->GetOwner() && other->GetOwner()->GetComponent<PlayerController>()))
            return;
        
        is_active = false;
    }
};

class ProjectilePool
{
    std::stack<std::shared_ptr<Projectile>> pool;

public:
    std::vector<std::shared_ptr<Projectile>> active_projectiles;

    std::shared_ptr<Projectile> Get()
    {
        auto proj = pool.top();
        pool.pop();
        return proj;
    }

    void Return(const std::shared_ptr<Projectile>& proj)
    {
        pool.push(proj);
    }

    void ReturnAll()
    {
        auto it = active_projectiles.begin();
        while (it != active_projectiles.end())
        {
            Return(*it);
            it = active_projectiles.erase(it);
        }
    }

    void Add(const size_t count, const int width, const int height, const std::shared_ptr<CollisionSolver>& collision_solver)
    {
        for (size_t i = 0; i < count; ++i)
        {
            std::shared_ptr<Projectile> projectile = std::make_shared<Projectile>(width, height);
            collision_solver->AddCollider(projectile);
            pool.push(projectile);
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
    std::shared_ptr<ProjectilePool> projectile_pool;
    std::shared_ptr<CollisionSolver> collision_solver;

    uint32_t proj_life_time_ms = 0;
    float proj_speed = 0;
    size_t pool_size = 0;

    uint32_t last_emit_time = 0;
    uint32_t emit_delay_ms = 0;

    int proj_width = 0;
    int proj_height = 0;
    std::string proj_sprite_sheet_filepath;
    SDL_Texture* proj_sprite_sheet = nullptr;
    std::shared_ptr<class Transform> transform;
    std::shared_ptr<class Map> map;

    static int GetDirectionIndex(const Vector2& direction);

public:
    ProjectileEmitter(std::string proj_sprite_sheet_filepath,
                      const int proj_width, const int proj_height,
                      const float proj_speed, const uint32_t proj_life_time_ms,
                      const size_t pool_size, const uint32_t emit_delay_ms,
                      const std::shared_ptr<Map>& map, const std::shared_ptr<CollisionSolver>& collision_solver)
        : collision_solver(collision_solver), proj_life_time_ms(proj_life_time_ms), proj_speed(proj_speed),
          pool_size(pool_size), emit_delay_ms(emit_delay_ms), proj_width(proj_width),
          proj_height(proj_height), proj_sprite_sheet_filepath(std::move(proj_sprite_sheet_filepath)), map(map)
    {
        projectile_pool = std::make_shared<ProjectilePool>();
    }

    void Setup() override;
    void Update(float delta_time) override;
    void Render() override;
    void Reset() override;

    void Emit(const Vector2& direction);
};
