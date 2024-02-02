#pragma once

#include <unordered_map>
#include <memory>
#include "entity.h"
#include "Components/transform.h"

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

    const std::unordered_map<int, std::shared_ptr<Entity>>& GetEntities()
    {
        return entities;
    }
    
    void AddEntity(const std::shared_ptr<Entity>& entity)
    {
        if (!entity)
            return;

        entities.try_emplace(entity->GetId(), entity);
    }

    const std::shared_ptr<Entity>& GetEntityById(const int id)
    {
        if (const auto it = entities.find(id); it != entities.end())
            return it->second;
        
        return nullptr;
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

    void LoadTransitionData(const std::shared_ptr<TransitionStorage>& transition_storage) const
    {
        for (const auto& entity : entities)
        {
            if (std::shared_ptr<TransitionData> transition_data;
                transition_storage->TryGetTransitionData(entity.first, transition_data))
            {
                entity.second->GetComponent<Transform>()->SetPosition(transition_data->position.x, transition_data->position.y);
            }
        }
    }
};
