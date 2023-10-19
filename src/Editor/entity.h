#pragma once

#include <string>
#include <vector>

#include "components.h"

class Entity
{
public:
    Entity(const char* name) : name(name), components({})
    {
        AddComponent<Transform>(.0f,.0f);
    }

    template <typename T, typename... TArgs>
    void AddComponent(TArgs&&... args)
    {
        T* component = new T(std::forward<TArgs>(args)...); // NOLINT(clang-diagnostic-implicit-int-float-conversion)
        components.push_back(component);
    }

    template <typename T>
    void RemoveComponent()
    {
        for (auto it = components.begin(); it != components.end(); ++it)
        {
            if (dynamic_cast<T*>(*it) != nullptr)
            {
                components.erase(it);
                break;
            }
        }
    }

    std::vector<Component*> GetComponents() const
    {
        return components;
    }

    std::string GetName() const
    {
        return name;
    }
    
    void SetName(const char* new_name)
    {
        name = new_name;
    }
    
private:
    std::string name;
    std::vector<Component*> components;
};
