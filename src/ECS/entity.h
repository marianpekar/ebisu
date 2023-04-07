#pragma once

#include <vector>
#include <unordered_map>
#include <typeindex>
#include <stdexcept>

#include "component_manager.h"
#include "Components/component.h"

class Entity 
{
private:
    bool is_active;
    std::unordered_map<std::type_index, Component*> component_map;
    ComponentManager* component_manager;
    const char* name;
public:
    Entity(const char* name, ComponentManager* component_manager) :
        name(name), component_manager(component_manager), is_active(true) {}
    ~Entity() = default;
    const bool& IsActive() const { return is_active; }
    const char* GetName() const { return name; }

    template <typename T, typename... TArgs>
    T* AddComponent(TArgs&&... args)
    {
        T* component = new T(std::forward<TArgs>(args)...);
        component->owner = this;
        component_map[typeid(T)] = component;
        component_manager->AddComponent(component);
        return component;
    }

    template <typename T>
    T* GetComponent()
    {
        auto component = component_map[typeid(T)];
        if (component != nullptr)
        {
            return dynamic_cast<T*>(component);
        }

        return nullptr;
    }
};