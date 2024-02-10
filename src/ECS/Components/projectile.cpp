#include "projectile.h"
#include "player_controller.h"

Projectile::Projectile(const int width, const int height)
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

void Projectile::Reset()
{
    is_active = false;
    destroy_time = 0;
}

void Projectile::Collide(const std::shared_ptr<Collider>& other, const Vector2& overlap, bool one_is_trigger)
{
    if (!is_active || (other->GetOwner() && other->GetOwner()->GetComponent<PlayerController>()))
        return;

    Reset();
}

Projectile::~Projectile()
{
    delete dst_rect;
    delete src_rect;
}
