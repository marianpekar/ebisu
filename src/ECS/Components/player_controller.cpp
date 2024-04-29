#include <SDL.h>
#include "../../game.h"
#include "../entity.h"
#include "player_controller.h"
#include "transform.h"
#include "rigidbody.h"
#include "character_animator.h"
#include "projectile_emitter.h"

void PlayerController::Setup()
{
    character_animator = owner->GetComponent<CharacterAnimator>();
    rigidbody = owner->GetComponent<Rigidbody>();
    projectile_emitter = owner->GetComponent<ProjectileEmitter>();
}

void PlayerController::Update(float delta_time)
{
    SDL_Event event;

    const Uint8* keys = SDL_GetKeyboardState(nullptr);

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            game->Quit();
            break;

        case SDL_KEYDOWN:
            if (keys[SDL_SCANCODE_W])
            {
                move_dir.y = -1;
            }

            if (keys[SDL_SCANCODE_S])
            {
                move_dir.y = 1;
            }

            if (keys[SDL_SCANCODE_A])
            {
                move_dir.x = -1;
            }

            if (keys[SDL_SCANCODE_D])
            {
                move_dir.x = 1;
            }
            break;

        case SDL_KEYUP:
            if (keys[SDL_SCANCODE_W])
            {
                move_dir.y = -1;
            }
            else if (keys[SDL_SCANCODE_S])
            {
                move_dir.y = 1;
            }
            else
            {
                move_dir.y = 0;
            }

            if (keys[SDL_SCANCODE_A])
            {
                move_dir.x = -1;
            }
            else if (keys[SDL_SCANCODE_D])
            {
                move_dir.x = 1;
            }
            else
            {
                move_dir.x = 0;
            }

            break;

        default:
            break;
        }
    }

    move_dir.Normalize();

    const Vector2 move_force = move_dir * (move_speed * delta_time);
    rigidbody->AddForce(move_force);

    if (character_animator != nullptr)
    {
        character_animator->SetMoveDirection(move_dir);
    }

    if (abs(move_dir.x) > 0.0f || abs(move_dir.y) > 0.0f)
    {
        facing_dir = move_dir;
    }

    if (keys[SDL_SCANCODE_SPACE] && projectile_emitter != nullptr)
    {
        projectile_emitter->Emit(facing_dir);
    }
}

void PlayerController::SetMoveSpeed(const float player_move_speed)
{
    this->move_speed = player_move_speed;
}
