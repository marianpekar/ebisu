#include "entity.h"
#include "entity_manager.h"
#include "Components/component.h"

Entity::Entity(const char* name, EntityManager* entityManager) : name(name), is_active(true) 
{
    entityManager->AddEntity(this);
}

void Entity::Setup()
{
    for (auto& component : components)
    {
        component->Setup();
    }
}

void Entity::Update(float deltaTime) 
{
    for (auto& component : components) 
    {
        component->Update(deltaTime);
    }
}

void Entity::Render() 
{
    for (auto& component : components) 
    {
        component->Render();
    }
}

Entity::~Entity() 
{
    for (auto& component : components)
    {
        delete component;
    }
}