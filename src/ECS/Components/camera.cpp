#include "camera.h"
#include "transform.h"
#include "../entity.h"

void Camera::Setup()
{
    target = owner->GetComponent<Transform>();
}

void Camera::Update(float delta_time)
{
    const Vector2 target_position = target->GetPosition();
    position.x = target_position.x - static_cast<float>(screen_half_width);
    position.y = target_position.y - static_cast<float>(screen_half_height);
}
