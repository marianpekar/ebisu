#pragma once

#include <vector>
#include <unordered_map>
#include <typeindex>
#include <stdexcept>

#include "Components/transform.h"

class EntityManager;
class Entity 
{
private:
    bool is_active;
    std::vector<Component*> components;
    std::unordered_map<std::type_index, Component*> component_map;
    const char* name;
public:
    Entity(const char* name, EntityManager* entityManager);
    ~Entity();
    void Update(float delta_time);
    void Render();
    const bool& IsActive() const { return is_active; }
    const char* GetName() const { return name; }

    template <typename T>
    T* AddComponent() 
    {
        T* component = new T();
        component->owner = this;
        components.emplace_back(component);
        component_map[typeid(T)] = component;
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

        throw std::runtime_error("[Entity] Entity " + std::string(name) + " has no component of type " + typeid(T).name());
    }
};