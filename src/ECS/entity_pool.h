#pragma once

#include "entity.h"
#include <unordered_map>

class EntityPool
{
private:
    std::unordered_map<int, Entity*> entities;
public:
    EntityPool() = default;
    
    ~EntityPool()
    {
        for (auto& entity : entities)
        {
            delete entity.second;
        }
        entities.clear();
    }

    void AddEntity(Entity* entity)
    {
        if (!entity)
            return;
        
        entities.emplace(entity->GetId(), entity);
    }

    Entity* GetEntityById(const int id)
    {
        const auto it = entities.find(id);
        return (it != entities.end()) ? it->second : nullptr;
    }

    void RemoveAllButPersistent()
    {
        auto it = entities.begin();
        while (it != entities.end())
        {
            if (!(it->second->IsPersistent()))
            {
                delete it->second;
                it = entities.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void RemoveEntity(const int id)
    {
        auto it = entities.find(id);
        if (it != entities.end())
        {
            delete it->second;
            entities.erase(it);
        }
    }
};
