#include "component_manager.h"

#include "entity.h"
#include "Components/component.h"

void ComponentManager::AddComponent(const std::shared_ptr<Component>& component)
{
    components.emplace_back(component);
}

void ComponentManager::RemoveAllButPersistent()
{
    auto it = components.begin();
    while (it != components.end())
    {
        if (!(*it)->GetIsPersistent())
        {
            it = components.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void ComponentManager::ResetComponents() const
{
    for (const auto& component : components)
    {
        component->Reset();
    }
}

void ComponentManager::Setup() const
{
    for (auto& component : components)
    {
        component->Setup();
    }
}

void ComponentManager::Update(const float delta_time) const
{
    for (auto& component : components)
    {
        if (!component->owner->IsActive())
            continue;
        
        component->Update(delta_time);
    }
}

void ComponentManager::FixedUpdate(float fixed_delta_time)
{
    for (auto& component : components)
    {
        if (!component->owner->IsActive())
            continue;
        
        component->FixedUpdate(fixed_delta_time);
    }
}

void ComponentManager::Render() const
{
    for (auto& component : components)
    {
        if (!component->owner->IsActive())
            continue;
        
        component->Render();
    }
}

ComponentManager::~ComponentManager()
{
    components.clear();
}
