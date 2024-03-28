#include "camera.h"
#include "../../renderer.h"
#include "transform.h"
#include "../entity.h"

void Camera::Setup()
{
    target = owner->GetComponent<Transform>();

    Renderer::GetScale(scale_x, scale_y);
}

void Camera::Update(float delta_time)
{
    const Vector2 target_position = target->GetPosition();
    position.x = target_position.x * scale_x - static_cast<float>(screen_half_width);
    position.y = target_position.y * scale_y - static_cast<float>(screen_half_height);
}
