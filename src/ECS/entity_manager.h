#pragma once

#include <vector>

class Entity;
class EntityManager 
{
private:
    std::vector<Entity*> entities;
public:
    void Update(float delta_time);
    void Render();
    void AddEntity(Entity* entity);
};