#pragma once

#include <vector>

class ComponentManager 
{
private:
    std::vector<class Component*> components;
public:
    ~ComponentManager();
    void Setup();
    void Update(float delta_time);
    void Render();
    void AddComponent(Component* component);
};