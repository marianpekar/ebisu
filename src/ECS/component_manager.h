#pragma once

#include <vector>
#include <memory>

class ComponentManager 
{
private:
    std::vector<std::shared_ptr<class Component>> components;
    void SortComponents();
public:
    ~ComponentManager();
    void Setup();
    void Update(float delta_time) const;
    void FixedUpdate(float fixed_delta_time);
    void Render() const;
    void AddComponent(const std::shared_ptr<Component>& component);
    void RemoveAllButPersistent();
    void ResetComponents() const;
};