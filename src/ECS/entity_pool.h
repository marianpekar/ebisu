#pragma once

#include <unordered_map>
#include <memory>
#include "entity.h"

class EntityPool
{
private:
    std::unordered_map<int, std::shared_ptr<Entity>> entities;

public:
    EntityPool() = default;

    ~EntityPool()
    {
        entities.clear();
    }

    void AddEntity(const std::shared_ptr<Entity>& entity)
    {
        if (!entity)
            return;

        entities.emplace(entity->GetId(), entity);
    }

    const std::shared_ptr<Entity>& GetEntityById(const int id)
    {
        const auto it = entities.find(id);
        return it != entities.end() ? it->second : nullptr;
    }

    void RemoveAllButPersistent()
    {
        auto it = entities.begin();
        while (it != entities.end())
        {
            if (!(it->second->IsPersistent()))
            {
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
            it->second.reset();
            entities.erase(it);
        }
    }
};
