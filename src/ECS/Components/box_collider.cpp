#include "../collision_solver.h"
#include "../entity.h"
#include "box_collider.h"

#include <SDL.h>
#include "../../renderer.h"

#include "transform.h"
#include "map_collider.h"
#include "rigidbody.h"

BoxCollider::BoxCollider(const float width_val, const float height_val, const float offset_x_val, const float offset_y_val,
    const bool is_trigger_val, const std::shared_ptr<CollisionSolver>& collision_solver) : collision_solver(collision_solver)
{
    position = Vector2::Zero();
    is_trigger = is_trigger_val;
    width = width_val;
    height = height_val;
    half_width = width_val / 2;
    half_height = height_val / 2;
    offset_x = offset_x_val;
    offset_y = offset_y_val;
}

void BoxCollider::Setup()
{
    transform = owner->GetComponent<Transform>();
    map_collider = owner->GetComponent<MapCollider>();
    rigidbody = owner->GetComponent<Rigidbody>();
    collision_solver->AddCollider(shared_from_this());
}

void BoxCollider::Update(float delta_time)
{
    position = Vector2(transform->GetPosition().x + offset_x, transform->GetPosition().y + offset_y);
}

void BoxCollider::Collide(const std::shared_ptr<Collider>& other, const Vector2& overlap, const bool one_is_trigger)
{
    if (on_collision != nullptr && collision_user_data != nullptr)
    {
        on_collision(other, collision_user_data);
    }
    
    if (one_is_trigger)
        return;
    
    const Vector2 current_pos = transform->GetPosition();
    Vector2 target_pos = current_pos + overlap / 2.0f;

    if (map_collider != nullptr)
    {
        map_collider->AdjustTargetPosition(current_pos, target_pos);
    }
    transform->SetPosition(target_pos);

    if (rigidbody != nullptr && other->GetRigidbody() != nullptr)
    {
        const Vector2 force = rigidbody->GetForce();
        other->GetRigidbody()->AddForce(force);
        rigidbody->AddForce(-force);
    }
}

#if _DEBUG
void BoxCollider::Render()
{
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(Renderer::GetRenderer(), &r, &g, &b, &a);
    
    SDL_SetRenderDrawColor(Renderer::GetRenderer(), 255, 255, 0, 0);
    
    const auto* rect = new SDL_Rect{
        static_cast<int>(position.x - Renderer::TryGetCameraPosition().x),
        static_cast<int>(position.y - Renderer::TryGetCameraPosition().y),
        static_cast<int>(width), static_cast<int>(height)
    };
    SDL_RenderDrawRect(Renderer::GetRenderer(), rect);
    delete rect;
    
    SDL_SetRenderDrawColor(Renderer::GetRenderer(), r, g, b, a);
}
#endif