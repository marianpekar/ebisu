#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <typeindex>

#include "component_manager.h"
#include "Components/component.h"

class Entity : public std::enable_shared_from_this<Entity>
{
private:
    bool is_active = true;
    bool is_persistent = false;
    std::unordered_map<std::type_index, std::shared_ptr<Component>> component_map;
    std::shared_ptr<ComponentManager> component_manager;
    int id = -1;
    std::string name;
public:
    Entity(int id, std::string name, const std::shared_ptr<ComponentManager>& component_manager) :
        component_manager(component_manager), id(id), name(std::move(name)) {}
    ~Entity() = default;
    
    const std::string& GetName() const { return name; }
    int GetId() const { return id; }

    const bool& IsActive() const { return is_active; }
    void SetIsActive(const bool active) { this->is_active = active; }

    const bool& IsPersistent() const { return is_persistent; }
    void SetIsPersistent(const bool persistent) { this->is_persistent = persistent; }
    
    template <typename T, typename ... TArgs>
    std::shared_ptr<T> AddComponent(TArgs&&... args)
    {
        std::shared_ptr<T> component = std::make_shared<T>(std::forward<TArgs>(args)...);  // NOLINT(clang-diagnostic-implicit-int-float-conversion)
        component->owner = shared_from_this();
        component->SetIsPersistent(is_persistent);
        component_map[typeid(T)] = component;
        component_manager->AddComponent(component);
        return component;
    }

    template <typename T>
    void AssignComponent(const std::shared_ptr<T>& component)
    {
        component->owner = shared_from_this();
        component->SetIsPersistent(is_persistent);
        component_map[typeid(T)] = component;
        component_manager->AddComponent(component);
    }

    template <typename T>
    std::shared_ptr<T> GetComponent()
    {
        if (const auto it = component_map.find(typeid(T)); it != component_map.end())
        {
            return std::dynamic_pointer_cast<T>(it->second);
        }

        return nullptr;
    }

};
