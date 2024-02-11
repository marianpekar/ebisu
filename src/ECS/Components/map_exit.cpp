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

void MapExit::OnCollisionStatic(const std::shared_ptr<Collider>& other, void* user_data)
{
    const MapExit* map_exit = static_cast<MapExit*>(user_data);
    map_exit->ChangeLevel(other, map_exit->game);
}

void MapExit::ChangeLevel(const std::shared_ptr<Collider>& other, Game* game_ptr) const
{
    const std::shared_ptr<Entity> other_entity = other->GetOwner();
    if (!other_entity)
        return;
    
    if (!other_entity->GetComponent<PlayerController>())
        return;
    
    other_entity->GetComponent<Transform>()->SetPosition(move_other_to_pos);
    
    game_ptr->ChangeLevel(next_map_path);
}