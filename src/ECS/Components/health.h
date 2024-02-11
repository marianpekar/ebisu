#pragma once

#include "component.h"

class Health final : public Component
{
private:
    float health;
    float max_health;
    void AddToHealth(float diff);
public:
    Health(float health, float max_health);
    void ReceiveDamage(float damage);
    void ReceiveAid(float aid);
    void Setup() override;
    void Die();
    
    static void OnProjectileHit(const std::shared_ptr<Entity>& owner_entity, void* user_data);
};
