#pragma once

#include "entity.h"
#include <unordered_map>

class EntityPool
{
private:
    std::vector<Entity*> entities;
    std::unordered_map<size_t, uint64_t> indices;

public:
    EntityPool() = default;

    void AddEntity(Entity* entity)
    {
        indices[entities.size()] = entity->GetId();
        entities.emplace_back(entity);
    }

    Entity* GetEntityById(const int id)
    {
        return entities[indices[id]];
    }
};