#pragma once

#include "entity.h"
#include <unordered_map>

class EntityPool
{
private:
    std::unordered_map<int, Entity*> entities;

public:
    EntityPool() = default;

    void AddEntity(Entity* entity)
    {
        entities[entity->GetId()] = entity;
    }

    Entity* GetEntityById(const int id)
    {
        return entities[id];
    }
};