#include <SDL.h>
#include "../../game.h"
#include "../entity.h"
#include "player_controller.h"
#include "transform.h"
#include "rigidbody.h"

void PlayerController::Setup()
{
    rigidbody = owner->GetComponent<Rigidbody>();
}

void PlayerController::Update(float delta_time)
{
    SDL_Event event;
    SDL_PollEvent(&event);
    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    switch (event.type)
    {
        case SDL_QUIT:
            game->Quit();
            break;

        case SDL_KEYDOWN:
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
    
    const Vector2 move_force = move_dir.Normalized() * move_speed;
    rigidbody->AddForce(move_force);
}

void PlayerController::SetMoveSpeed(const float player_move_speed)
{
    this->move_speed = player_move_speed;
}
