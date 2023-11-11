#include "agent.h"
#include <SDL.h>
#include "rigidbody.h"
#include "transform.h"
#include "../entity.h"
#include "../../map.h"

#if _DEBUG
#include "../../renderer.h"
#endif


void Agent::Setup()
{
    transform = owner->GetComponent<Transform>();
    rigidbody = owner->GetComponent<Rigidbody>();
}

void Agent::Update(float delta_time)
{
    current_path = map->FindPath(transform->GetPosition(), target_transform->GetPosition());

    if ((target_transform->GetPosition() - transform->GetPosition()).Length() <= stopping_distance ||
        current_path.size() < 2)
        return;

    const Vector2 direction = (current_path[1] - current_path[0]).Normalized();
    rigidbody->AddForce(direction * speed);
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
        SDL_RenderDrawLine(Renderer::GetRenderer(),
           static_cast<int>(current_path[i].x - Renderer::TryGetCameraPosition().x),
           static_cast<int>(current_path[i].y - Renderer::TryGetCameraPosition().y),
           static_cast<int>(current_path[i + 1].x - Renderer::TryGetCameraPosition().x),
           static_cast<int>(current_path[i + 1].y - Renderer::TryGetCameraPosition().y));
    }
    
    SDL_SetRenderDrawColor(Renderer::GetRenderer(), r, g, b, a);
}
#endif
