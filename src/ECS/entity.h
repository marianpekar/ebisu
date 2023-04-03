#pragma once

#include <vector>
#include <unordered_map>
#include <typeindex>

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
    void Update(float delta_time);
    void Render();
    void Destroy();
    const bool& IsActive() const { return is_active; }
    const char* GetName() const { return name; }

    template <typename T, typename... TArgs>
    T& AddComponent(TArgs&&... args) 
    {
        T* component = new T(std::forward<TArgs>(args)...);
        component->owner = this;
        components.emplace_back(component);
        component_map[typeid(T)] = component;
        return *component;
    }

    template <typename T>
    T* GetComponent()
    {
        auto component = component_map[typeid(T)];
        if (component != nullptr)
        {
            return dynamic_cast<T*>(component);
        }

        throw std::exception("[Entity] Component not found");
    }
};