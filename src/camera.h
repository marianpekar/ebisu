#pragma once

#include "Math/vector2.h"
#include "ECS/Components/transform.h"

class Camera
{
private:
    class Transform* target;
    Vector2 position;
    int screen_half_width, screen_half_height;
public:
    Camera(Transform* target, const int screen_width, const int screen_height ) : 
        target(target), position(),
        screen_half_width(screen_width / 2), screen_half_height(screen_height / 2) {}
    const Vector2& GetPosition() const { return position; }
    void Update()
    {
        const Vector2 target_position = target->GetPosition();
        position.x = target_position.x - static_cast<float>(screen_half_width);
        position.y = target_position.y - static_cast<float>(screen_half_height);
    }
};