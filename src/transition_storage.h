#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "Math/vector2.h"

class EntityPool;

struct TransitionData
{
    Vector2 position;
};

class TransitionStorage
{
private:
    std::string current_map_path;
    std::unordered_map<std::string, std::unordered_map<int, std::shared_ptr<TransitionData>>> storage;
    void AddTransitionData(int entity_id, const std::shared_ptr<TransitionData>& transition_data);
public:
    void SetCurrentMapPath(const std::string& map_path);
    void ClearTransitionData();
    bool TryGetTransitionData(int entity_id, std::shared_ptr<TransitionData>& transition_data) const;
    void SaveTransitionData(const std::shared_ptr<EntityPool>& entity_pool);
};


