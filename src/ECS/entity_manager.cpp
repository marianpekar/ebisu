#include "entity_manager.h"
#include "entity.h"
#include "Components/component.h"

void EntityManager::AddEntity(Entity* entity)
{
    entities.emplace_back(entity);
}

void EntityManager::Update(float deltaTime) 
{
    for (auto& entity : entities) 
    {
        entity->Update(deltaTime);
    }
}

void EntityManager::Render() 
{
    for (auto& entity : entities) 
    {
        entity->Render();
    }
}

EntityManager::~EntityManager()
{
    for (auto& entity : entities)
    {
        delete entity;
    }
}

