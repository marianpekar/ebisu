#pragma once

#include "component.h"

class Health final : public Component
{
private:
    std::shared_ptr<class CharacterAnimator> character_animator;
    std::shared_ptr<class Agent> agent;
    float health;
    float max_health;
    void AddToHealth(float diff);
public:
    Health(float health, float max_health);
    void ReceiveDamage(float damage);
    void ReceiveAid(float aid);
    void Setup() override;
    void Die() const;
    float GetHealth() const { return health; }
    void SetHealth(const float& val) { health = val; }
    
    static void OnProjectileHit(const std::shared_ptr<Entity>& owner_entity, void* user_data);
};
