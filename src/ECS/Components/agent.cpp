#include "agent.h"
#include <SDL.h>
#include "rigidbody.h"
#include "transform.h"
#include "character_animator.h"
#include "../entity.h"
#include "../../map.h"

#if _DEBUG
#include "../../renderer.h"
#endif


void Agent::Setup()
{
    transform = owner->GetComponent<Transform>();
    rigidbody = owner->GetComponent<Rigidbody>();
    character_animator = owner->GetComponent<CharacterAnimator>();
}

void Agent::Update(float delta_time)
{
    current_path = map->FindPath(transform->GetPosition(), target_transform->GetPosition());

    if ((target_transform->GetPosition() - transform->GetPosition()).Length() <= stopping_distance || current_path.size() < 2)
    {
        if (character_animator != nullptr)
        {
            character_animator->SetMoveDirection(Vector2(0,0));
        }
        
        return;
    }
    
    current_direction = (current_path[1] - current_path[0]).Normalized();
    rigidbody->AddForce(current_direction * speed);

    if (character_animator != nullptr)
    {
        character_animator->SetMoveDirection(current_direction);
    }
}

void Agent::SetTarget(Transform* target)
{
    target_transform = target;
}

#if _DEBUG
void Agent::Render()
{
    if (current_path.empty())
        return;
    
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(Renderer::GetRenderer(), &r, &g, &b, &a);

    SDL_SetRenderDrawColor(Renderer::GetRenderer(), 0, 0, 255, 255);
    for (size_t i = 0; i < current_path.size() - 1; i++)
    {
        Renderer::DrawLine(current_path[i], current_path[i+1]);
    }
    
    SDL_SetRenderDrawColor(Renderer::GetRenderer(), r, g, b, a);
}
#endif
