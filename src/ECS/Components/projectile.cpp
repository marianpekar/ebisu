#include "projectile.h"
#include "player_controller.h"
#include "projectile_acceptor.h"

Projectile::Projectile(const int width_val, const int height_val, const int frames) : frames(frames)
{
    dst_rect = new SDL_Rect();
    src_rect = new SDL_Rect();
    src_rect->x = 0;
    src_rect->y = 0;
    src_rect->w = width_val;
    src_rect->h = height_val;
    is_trigger = true;
    width = width_val;
    height = height_val;
    half_width = width_val / 2;
    half_height = height_val / 2;
}

void Projectile::NextFrame()
{
    current_frame = (current_frame + 1) % frames;
    SelectSpriteCol(current_frame);
}

void Projectile::SelectSpriteCol(const int& col) const
{
    src_rect->x = width * col;
}

void Projectile::SelectSpriteRow(const int& row) const
{
    src_rect->y = height * row;
}

void Projectile::Reset()
{
    is_active = false;
    current_frame = 0;
    SelectSpriteCol(current_frame);
    destroy_time = 0;
}

void Projectile::Collide(const std::shared_ptr<Collider>& other, const Vector2& overlap, bool one_is_trigger)
{
    if (!is_active)
        return;

    const std::shared_ptr<Entity> other_entity = other->GetOwner();
    if (!other_entity)
        return;

    const std::shared_ptr<ProjectileAcceptor> projectile_acceptor = other_entity->GetComponent<ProjectileAcceptor>();
    if (!projectile_acceptor)
        return;
    
    projectile_acceptor->Accept(other_entity);
    is_active = false;
}

Projectile::~Projectile()
{
    delete dst_rect;
    delete src_rect;
}