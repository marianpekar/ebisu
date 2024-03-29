﻿#pragma once

#include <string>
#include <vector>

#include "components.h"

class Entity
{
public:
    Entity(const char* name, size_t index, uint64_t id) : is_active(true), index(index), id(id), name(name), components({})
    {
        AddComponent<Transform>(.0f,.0f);
    }

    template <typename T, typename... TArgs>
    void AddComponent(TArgs&&... args)
    {
        T* component = new T(std::forward<TArgs>(args)...); // NOLINT(clang-diagnostic-implicit-int-float-conversion)
        components.push_back(component);
    }

    void RemoveComponent(const size_t index)
    {
        delete components[index];
        components.erase(components.begin() + static_cast<int>(index));
    }

    Component* GetComponent(const char* name) const
    {
        for (const auto& component : components)
        {
            if (component->GetName() == name)
            {
                return component;
            }
        }

        return nullptr;
    }

    std::vector<Component*> GetComponents() const { return components; }
    
    std::string GetName() const { return name; }
    void SetName(const char* new_name) { name = new_name; }

    size_t GetIndex() { return index; }
    uint64_t GetId() { return id; }
    
    bool is_active;
    bool is_persistent;
private:
    size_t index;
    uint64_t id;
    std::string name;
    std::vector<Component*> components;
};
