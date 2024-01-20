#pragma once

#include <memory>
#include "component.h"
#include "../../Math/vector2.h"
#include "transform.h"

class Camera final : public Component
{
private:
    std::shared_ptr<Transform> target;
    Vector2 position;
    int screen_half_width, screen_half_height;
public:
    Camera(const int screen_width, const int screen_height) :
        screen_half_width(screen_width / 2), screen_half_height(screen_height / 2) {}
    const Vector2& GetPosition() const { return position; }
    void Setup() override;
    void Update(float delta_time) override;
};
