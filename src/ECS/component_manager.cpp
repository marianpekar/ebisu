#include "component_manager.h"
#include "Components/component.h"

void ComponentManager::AddComponent(Component* component)
{
    components.emplace_back(component);
}

void ComponentManager::Setup()
{
    for (auto& component : components)
    {
        component->Setup();
    }
}


void ComponentManager::Update(float deltaTime)
{
    for (auto& component : components)
    {
        component->Update(deltaTime);
    }
}

void ComponentManager::Render()
{
    for (auto& component : components)
    {
        component->Render();
    }
}

ComponentManager::~ComponentManager()
{
    for (auto& component : components)
    {
        delete component;
    }
}
