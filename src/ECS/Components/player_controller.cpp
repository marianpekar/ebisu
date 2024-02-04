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

    if(keys[SDL_SCANCODE_SPACE] && projectile_emitter != nullptr)
    {
        projectile_emitter->Emit(current_dir.Normalized());
    }

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            game->Quit();
            break;

        case SDL_KEYDOWN:
            if(keys[SDL_SCANCODE_W])
            {
                move_dir.y = -1;
                current_dir.y = move_dir.y;
            }
            else if(keys[SDL_SCANCODE_S])
            {
                move_dir.y = 1;
                current_dir.y = move_dir.y;
            }
            else if(keys[SDL_SCANCODE_A])
            {
                move_dir.x = -1;
                current_dir.x = move_dir.x;
            }
            else if(keys[SDL_SCANCODE_D])
            {
                move_dir.x = 1;
                current_dir.x = move_dir.x;
            }

            break;

        case SDL_KEYUP:
            if(event.key.keysym.scancode == SDL_SCANCODE_W || event.key.keysym.scancode == SDL_SCANCODE_S)
            {
                move_dir.y = 0;
            }

            if(event.key.keysym.scancode == SDL_SCANCODE_A || event.key.keysym.scancode == SDL_SCANCODE_D)
            {
                move_dir.x = 0;
            }

            break;

        default:
            break;
        }
    }

    const Vector2 move_force = move_dir.Normalized() * move_speed;
    rigidbody->AddForce(move_force);

    if(character_animator != nullptr)
    {
        character_animator->SetMoveDirection(move_dir);
    }
}

void PlayerController::SetMoveSpeed(const float player_move_speed)
{
    this->move_speed = player_move_speed;
}
