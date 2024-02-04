#include "projectile_emitter.h"
#include <SDL.h>
#include "../../texture_loader.h"
#include "../../renderer.h"
#include "../entity.h"
#include "transform.h"

void ProjectileEmitter::Setup()
{
    transform = owner->GetComponent<Transform>();
    proj_sprite = TextureLoader::LoadTexture(proj_sprite_filepath, Renderer::GetRenderer(), proj_width, proj_height);

    projectile_pool.Add(pool_size);
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
        }
        else
        {
            (*it)->position += (*it)->direction * (*it)->speed * delta_time;
            ++it;
        }
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

        SDL_RenderCopy(Renderer::GetRenderer(), proj_sprite, nullptr, proj->dst_rect);
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

    active_projectiles.emplace_back(proj);

    last_emit_time = SDL_GetTicks();
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
