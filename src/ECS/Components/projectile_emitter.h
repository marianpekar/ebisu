#pragma once

#include <SDL.h>
#include <string>
#include <memory>

#include "component.h"
#include "player_controller.h"
#include "../collision_solver.h"
#include "../../Math/vector2.h"
#include "projectile_pool.h"

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

    uint32_t animation_frame_time;
    int animation_frames;
    uint32_t last_time = 0;

    static int GetDirectionIndex(const Vector2& direction);

public:
    ProjectileEmitter(std::string proj_sprite_sheet_filepath,
                      const int proj_width, const int proj_height,
                      const float proj_speed, const uint32_t proj_life_time_ms,
                      const size_t pool_size, const uint32_t emit_delay_ms,
                      const std::shared_ptr<Map>& map, const std::shared_ptr<CollisionSolver>& collision_solver,
                      const uint32_t animation_frame_time, const int animation_frames)
        : collision_solver(collision_solver), proj_life_time_ms(proj_life_time_ms), proj_speed(proj_speed),
          pool_size(pool_size), emit_delay_ms(emit_delay_ms), proj_width(proj_width),
          proj_height(proj_height), proj_sprite_sheet_filepath(std::move(proj_sprite_sheet_filepath)), map(map),
          animation_frame_time(animation_frame_time), animation_frames(animation_frames)
    {
        projectile_pool = std::make_shared<ProjectilePool>();
    }

    void Setup() override;
    void Update(float delta_time) override;
    void Render() override;
    void Reset() override;

    void Emit(const Vector2& direction);
};
