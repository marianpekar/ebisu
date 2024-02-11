#pragma once
#include "component.h"

class DamageReceptor : public Component
{
private:
    float on_projectile_hit;
public:
    DamageReceptor(const float on_projectile_hit) : on_projectile_hit(on_projectile_hit) {}
    const float& GetOnProjectileHitDamage() const { return on_projectile_hit; }
};
