#pragma once

#include "../../Math/vector2.h"
#include "component.h"

class PlayerController : public Component
{
private:
    class Game* game;
    float move_speed = 4096.f;
    Vector2 move_dir;

    class Rigidbody* rigidbody = nullptr;
public:
    PlayerController(Game* game) : game(game) {}
    ~PlayerController() override = default;
    void Setup() override;
    void Update(float delta_time) override;
    void SetMoveSpeed(float player_move_speed);
};