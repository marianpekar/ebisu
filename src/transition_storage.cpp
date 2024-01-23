#include "transition_storage.h"

#include "ECS/entity_pool.h"

void TransitionStorage::SetCurrentMapPath(const std::string& map_path)
{
    current_map_path = map_path;
}

void TransitionStorage::ClearTransitionData()
{
    if(!storage.contains(current_map_path))
        return;
    
    storage.at(current_map_path).clear();
}

void TransitionStorage::AddTransitionData(int entity_id, const std::shared_ptr<TransitionData>& transition_data)
{
    if(!storage.contains(current_map_path))
    {
        std::unordered_map<int, std::shared_ptr<TransitionData>> map_storage;
        storage.try_emplace(current_map_path, map_storage);
    }
    
    storage.at(current_map_path).try_emplace(entity_id, transition_data);
}

bool TransitionStorage::TryGetTransitionData(int entity_id, std::shared_ptr<TransitionData>& transition_data) const
{
    if(!storage.contains(current_map_path))
        return false;

    const auto& map_storage = storage.at(current_map_path);
    if (!map_storage.contains(entity_id))
        return false;
        
    transition_data = map_storage.at(entity_id);
    return true;
}

void TransitionStorage::SaveTransitionData(const std::shared_ptr<EntityPool>& entity_pool)
{
    ClearTransitionData();
    for (const auto& entity : entity_pool->GetEntities())
    {
        std::shared_ptr<Transform> transform = entity.second->GetComponent<Transform>();
        if (transform != nullptr)
        {
            std::shared_ptr<TransitionData> transition_data = std::make_shared<TransitionData>();
            transition_data->position = Vector2{ transform->GetPosition().x, transform->GetPosition().y };
            AddTransitionData(entity.first, transition_data);
        }
    }
}
