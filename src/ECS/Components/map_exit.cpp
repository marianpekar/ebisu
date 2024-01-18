#include "map_exit.h"
#include "box_collider.h"
#include "player_controller.h"
#include "transform.h"
#include "../entity.h"
#include "../../game.h"

void MapExit::Setup()
{
    collider = owner->GetComponent<BoxCollider>();
    
    collider->on_collision = OnCollisionStatic;
    collider->collision_user_data = this;
}

void MapExit::ChangeLevel(const BoxCollider* other, Game* game) const
{
    if (other->owner->GetComponent<PlayerController>() == nullptr)
        return;

    Transform* other_transform = other->owner->GetComponent<Transform>();
    other_transform->SetPosition(*move_other_to_pos);

    game->ChangeLevel(next_map_path);
}

void MapExit::OnCollisionStatic(const BoxCollider* other, void* user_data) {
    MapExit* map_exit = static_cast<MapExit*>(user_data);
    map_exit->ChangeLevel(other, map_exit->game);
}

MapExit::~MapExit()
{
    delete move_other_to_pos;
}
