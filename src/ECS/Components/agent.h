#pragma once

#include <vector>

#include "../../path_node.h"
#include "component.h"

class Agent final : public Component
{
private:
    class Map* map;
    class Transform* transform = nullptr;
    Transform* target_transform = nullptr;
    std::vector<PathNode*> current_path;
    
public:
    Agent(Map* map) : map(map) {}
    void Setup() override;
    void Update(float delta_time) override;
    void SetTarget(Transform* target);
};
