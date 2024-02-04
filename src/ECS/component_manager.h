#pragma once

#include <vector>
#include <memory>

class ComponentManager 
{
private:
    std::vector<std::shared_ptr<class Component>> components;
public:
    ~ComponentManager();
    void Setup() const;
    void Update(float delta_time) const;
    void Render() const;
    void AddComponent(const std::shared_ptr<Component>& component);
    void RemoveAllButPersistent();
    void ResetComponents() const;
};