#include "entity.h"
#include "entity_pool.h"

Entity::Entity(ComponentManager* component_manager, EntityPool* entity_pool) :
    is_active(true), is_persistent(false), component_manager(component_manager)
{
    entity_pool->AddEntity(this);
}