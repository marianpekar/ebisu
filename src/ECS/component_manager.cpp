#include "component_manager.h"

#include <algorithm>

#include "entity.h"
#include "Components/component.h"
#include "Components/sprite_sheet.h"

void ComponentManager::AddComponent(const std::shared_ptr<Component>& component)
{
    components.emplace_back(component);
}

void ComponentManager::SortComponents()
{
    std::sort(components.begin(), components.end(),
        [](const std::shared_ptr<Component>& a, const std::shared_ptr<Component>& b) {
        std::string typeA = typeid(*a).name();
        std::string typeB = typeid(*b).name();

        if(typeA == typeB && typeA == typeid(SpriteSheet).name()) {
            std::shared_ptr<SpriteSheet> spriteA = std::dynamic_pointer_cast<SpriteSheet>(a);
            std::shared_ptr<SpriteSheet> spriteB = std::dynamic_pointer_cast<SpriteSheet>(b);

            return spriteA->GetZIndex() < spriteB->GetZIndex();
        }

        return typeA < typeB;
    });
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

void ComponentManager::Setup()
{
    SortComponents();
    
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
