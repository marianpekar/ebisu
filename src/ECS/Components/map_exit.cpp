#include "map_exit.h"
#include "box_collider.h"
#include "player_controller.h"
#include "transform.h"
#include "../entity.h"
#include "../../game.h"

void MapExit::Setup()
{
    collider = owner->GetComponent<BoxCollider>();
    collider->on_collision = [this](const BoxCollider* other)
    {
        ChangeLevel(other);
    };
}

void MapExit::ChangeLevel(const BoxCollider* other) const
{
    if(other->owner->GetComponent<PlayerController>() == nullptr)
        return;

    Transform* other_transform = other->owner->GetComponent<Transform>();
    other_transform->SetPosition(*move_other_to_pos);

    // TODO: Unload current and load next level via Game 
}

MapExit::~MapExit()
{
    delete move_other_to_pos;
}
