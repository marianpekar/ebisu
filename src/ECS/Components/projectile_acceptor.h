#pragma once

#include <vector>
#include <memory>
#include "component.h"
#include "../entity.h"

class ProjectileAcceptor final : public Component
{
    typedef void (*AcceptCallback)(const std::shared_ptr<Entity>&, void*);
    std::vector<std::pair<AcceptCallback, void*>> callbacks;
public:
    ProjectileAcceptor() = default;
    
    void RegisterCallback(AcceptCallback callback, void* user_data);
    void Accept(const std::shared_ptr<Entity>& entity) const;
};