#pragma once

#include "../../Math/vector2.h"
#include "component.h"
#include "../../game.h"

class PlayerController : public Component
{
private:
    Game* game;
    float move_speed;
    Vector2 move_dir;
    Vector2 facing_dir = Vector2(0,-1);
    std::shared_ptr<class CharacterAnimator> character_animator;
    std::shared_ptr<class Rigidbody> rigidbody;
    std::shared_ptr<class ProjectileEmitter> projectile_emitter;
public:
    PlayerController(Game* game, float speed) : game(game), move_speed(speed) {}
    ~PlayerController() override = default;
    void Setup() override;
    void Update(float delta_time) override;
    void SetMoveSpeed(float player_move_speed);
};