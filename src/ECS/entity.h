#pragma once

#include <string>
#include <unordered_map>
#include <typeindex>

#include "component_manager.h"
#include "Components/component.h"

class Entity 
{
private:
    bool is_active = true;
    bool is_persistent = false;
    std::unordered_map<std::type_index, Component*> component_map;
    ComponentManager* component_manager;
    int id = -1;
    std::string name;
public:
    Entity(int id, std::string name, ComponentManager* component_manager) :
        component_manager(component_manager), id(id), name(std::move(name)) {}
    
    std::string GetName() const { return name; }
    int GetId() const { return id; }

    const bool& IsActive() const { return is_active; }
    void SetIsActive(const bool active) { this->is_active = active; }

    const bool& IsPersistent() const { return is_persistent; }
    void SetIsPersistent(const bool persistent) { this->is_persistent = persistent; }
    
    template <typename T, typename ... TArgs>
    T* AddComponent(TArgs&&... args)
    {
        T* component = new T(std::forward<TArgs>(args)...);  // NOLINT(clang-diagnostic-implicit-int-float-conversion)
        component->owner = this;
        component->SetIsPersistent(is_persistent);
        component_map[typeid(T)] = component;
        component_manager->AddComponent(component);
        return component;
    }

    template <typename T>
    void AssignComponent(T* component)
    {
        component->owner = this;
        component->SetIsPersistent(is_persistent);
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

    ~Entity() = default;
};
