#pragma once

#include "../../Math/vector2.h"
#include "component.h"

class PlayerController : public Component
{
private:
    class Game* game;
    float move_speed;
    Vector2 move_dir;

    class Rigidbody* rigidbody = nullptr;
public:
    PlayerController(Game* game, float speed) : game(game), move_speed(speed) {}
    ~PlayerController() override = default;
    void Setup() override;
    void Update(float delta_time) override;
    void SetMoveSpeed(float player_move_speed);
};