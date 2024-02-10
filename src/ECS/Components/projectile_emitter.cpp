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

    projectile_pool->Add(pool_size, proj_width, proj_height, collision_solver);
}

void ProjectileEmitter::Update(float delta_time)
{
    auto it = projectile_pool->GetActiveProjectiles().begin();
    while (it != projectile_pool->GetActiveProjectiles().end())
    {
        if (SDL_GetTicks() > (*it)->GetDestroyTime())
        {
            projectile_pool->Return(*it);
            it = projectile_pool->GetActiveProjectiles().erase(it);
            continue;
        }

        const Vector2 new_position = (*it)->GetPosition() + (*it)->GetDirection() * (*it)->GetSpeed() * delta_time;
        (*it)->SetPosition(new_position.x, new_position.y);

        if (!(*it)->GetIsActive() || map->HasCollisionAt((*it)->GetPosition(), static_cast<float>(proj_width), static_cast<float>(proj_height)))
        {
            (*it)->Reset();
            projectile_pool->Return(*it);
            it = projectile_pool->GetActiveProjectiles().erase(it);
            continue;
        }
            
        ++it;
    }
}

void ProjectileEmitter::Render()
{
    for (const auto& proj : projectile_pool->GetActiveProjectiles())
    {
        proj->dst_rect->x = static_cast<int>(proj->GetPosition().x - Renderer::TryGetCameraPosition().x);
        proj->dst_rect->y = static_cast<int>(proj->GetPosition().y - Renderer::TryGetCameraPosition().y);
        proj->dst_rect->w = proj_width;
        proj->dst_rect->h = proj_height;

        SDL_RenderCopy(Renderer::GetRenderer(), proj_sprite_sheet, proj->src_rect, proj->dst_rect);
    }
}

void ProjectileEmitter::Emit(const Vector2& direction)
{
    if (last_emit_time + emit_delay_ms > SDL_GetTicks() || projectile_pool->IsEmpty())
        return;

    std::shared_ptr<Projectile> proj = projectile_pool->Get();
    proj->SetIsActive(true);
    proj->SetDestroyTime(SDL_GetTicks() + proj_life_time_ms);
    proj->SetPosition(transform->GetPosition().x, transform->GetPosition().y);
    proj->SetDirection(direction.x, direction.y);
    proj->SetSpeed(proj_speed);

    const int direction_index = GetDirectionIndex(direction);
    proj->src_rect->y = direction_index * proj_height;

    projectile_pool->GetActiveProjectiles().emplace_back(proj);

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
    projectile_pool->ReturnAll();
}
