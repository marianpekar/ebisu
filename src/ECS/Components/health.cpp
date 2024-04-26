#include "health.h"

#include "character_animator.h"
#include "damage_receptor.h"
#include "projectile_acceptor.h"
#include "agent.h"

Health::Health(float health, float max_health) : health(health), max_health(max_health) {}

void Health::Setup()
{
    if (const std::shared_ptr<ProjectileAcceptor> proj_acceptor = owner->GetComponent<ProjectileAcceptor>())
    {
       proj_acceptor->RegisterCallback(OnProjectileHit, this);
    }

    character_animator = owner->GetComponent<CharacterAnimator>();
    agent = owner->GetComponent<Agent>();
    
    if (health <= 0.0f)
    {
        if (character_animator != nullptr)
        {
            character_animator->PlayDeadAnimationLastFrame();
        }

        if (agent != nullptr)
        {
            agent->ForceStop();
        }
    }
    

}

void Health::OnProjectileHit(const std::shared_ptr<Entity>& owner_entity, void* user_data)
{
    const std::shared_ptr<DamageReceptor> damage_receptor = owner_entity->GetComponent<DamageReceptor>();
    if (!damage_receptor)
        return;
    
    static_cast<Health*>(user_data)->ReceiveDamage(damage_receptor->GetOnProjectileHitDamage());
}

void Health::AddToHealth(float diff)
{
    if (health <= 0.0f)
        return;

    health += diff;
    health = std::max(0.0f, std::min(max_health, health));

    if (health <= 0.0f)
        Die();
}

void Health::Die() const
{
    if (character_animator != nullptr)
    {
        character_animator->PlayDeadAnimation();
    }

    if (agent != nullptr)
    {
        agent->ForceStop();
    }
}

void Health::ReceiveDamage(const float damage)
{
    AddToHealth(-damage);
}

void Health::ReceiveAid(const float aid)
{
    AddToHealth(aid);
}