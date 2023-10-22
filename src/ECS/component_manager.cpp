#include "component_manager.h"

#include "entity.h"
#include "Components/component.h"

void ComponentManager::AddComponent(Component* component)
{
    components.emplace_back(component);
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
    for (const auto& component : components)
    {
        delete component;
    }
}
