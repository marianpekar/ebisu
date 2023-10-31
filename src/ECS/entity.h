#pragma once

#include <unordered_map>
#include <typeindex>

#include "component_manager.h"
#include "Components/component.h"

class Entity 
{
private:
    bool is_active;
    std::unordered_map<std::type_index, Component*> component_map;
    ComponentManager* component_manager;
    int id = -1;
    const char* name = nullptr;
public:
    Entity(ComponentManager* component_manager, class EntityPool* entity_pool);
    ~Entity() = default;
    
    const char* GetName() const { return name; }
    void SetName(const char* entity_name) { this->name = entity_name; }
    
    int GetId() const { return id; }
    void SetId(const int entity_id) { this->id = entity_id; }

    const bool& IsActive() const { return is_active; }
    void SetIsActive(const bool active) { this->is_active = active; }
    
    template <typename T, typename ... TArgs>
    T* AddComponent(TArgs&&... args)
    {
        T* component = new T(std::forward<TArgs>(args)...);  // NOLINT(clang-diagnostic-implicit-int-float-conversion)
        component->owner = this;
        component_map[typeid(T)] = component;
        component_manager->AddComponent(component);
        return component;
    }

    template <typename T>
    void AssignComponent(T* component)
    {
        component->owner = this;
        component_map[typeid(T)] = component;
        component_manager->AddComponent(component);
    }

    template <typename T>
    T* GetComponent()
    {
        if (auto component = component_map[typeid(T)]; component != nullptr)
        {
            return dynamic_cast<T*>(component);
        }

        return nullptr;
    }
};
