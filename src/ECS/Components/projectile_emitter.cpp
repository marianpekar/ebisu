#include "projectile_emitter.h"

#include <SDL.h>
#include "../../texture_loader.h"
#include "../../renderer.h"
#include "../entity.h"
#include "transform.h"
#include "../../map.h"

void ProjectileEmitter::Setup()
{
    transform = owner->GetComponent<Transform>();
    proj_sprite_sheet = TextureLoader::LoadTexture(proj_sprite_sheet_filepath, Renderer::GetRenderer(), proj_width,
                                                   proj_height);

    projectile_pool.Add(pool_size, proj_width, proj_height);
}

void ProjectileEmitter::Update(float delta_time)
{
    auto it = active_projectiles.begin();
    while (it != active_projectiles.end())
    {
        if (SDL_GetTicks() > (*it)->destroy_time)
        {
            projectile_pool.Return(*it);
            it = active_projectiles.erase(it);
            continue;
        }
        
        (*it)->position += (*it)->direction * (*it)->speed * delta_time;

        if (map->HasCollisionAt((*it)->position, static_cast<float>(proj_width), static_cast<float>(proj_height)))
        {
            projectile_pool.Return(*it);
            it = active_projectiles.erase(it);
            continue;
        }
            
        ++it;
    }
}

void ProjectileEmitter::Render()
{
    for (const auto& proj : active_projectiles)
    {
        proj->dst_rect->x = static_cast<int>(proj->position.x - Renderer::TryGetCameraPosition().x);
        proj->dst_rect->y = static_cast<int>(proj->position.y - Renderer::TryGetCameraPosition().y);
        proj->dst_rect->w = proj_width;
        proj->dst_rect->h = proj_height;

        SDL_RenderCopy(Renderer::GetRenderer(), proj_sprite_sheet, proj->src_rect, proj->dst_rect);
    }
}

void ProjectileEmitter::Emit(const Vector2& direction)
{
    if (last_emit_time + emit_delay_ms > SDL_GetTicks() || projectile_pool.IsEmpty())
        return;

    std::shared_ptr<Projectile> proj = projectile_pool.Get();
    proj->destroy_time = SDL_GetTicks() + proj_life_time_ms;
    proj->position = Vector2(transform->GetPosition().x, transform->GetPosition().y);
    proj->direction = Vector2(direction.x, direction.y);
    proj->speed = proj_speed;

    const int direction_index = GetDirectionIndex(direction);
    proj->src_rect->y = direction_index * proj_height;

    active_projectiles.emplace_back(proj);

    last_emit_time = SDL_GetTicks();
}

int ProjectileEmitter::GetDirectionIndex(const Vector2& direction)
{
    if (direction.x > 0)
    {
        if (direction.y > 0)
        {
            return 3; // down-right
        }
        if (direction.y < 0)
        {
            return 1; // up-right
        }
        return 2; // right
    }
    if (direction.x < 0)
    {
        if (direction.y > 0)
        {
            return 5; // down-left
        }
        if (direction.y < 0)
        {
            return 7; // up-left
        }
        return 6; // left
    }
    if (direction.y > 0)
    {
        return 4; // down
    }
    if (direction.y < 0)
    {
        return 0; // up
    }
    return -1;
}

void ProjectileEmitter::Reset()
{
    auto it = active_projectiles.begin();
    while (it != active_projectiles.end())
    {
        projectile_pool.Return(*it);
        it = active_projectiles.erase(it);
    }
}
