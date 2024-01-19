#pragma once

#include <vector>

class ComponentManager 
{
private:
    std::vector<class Component*> components;
public:
    ~ComponentManager();
    void Setup() const;
    void Update(float delta_time) const;
    void Render() const;
    void AddComponent(Component* component);
    void RemoveAllButPersistent();
};