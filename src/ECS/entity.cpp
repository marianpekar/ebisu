#include "entity.h"
#include "Components/transform.h"
#include "Components/component.h"
#include "entity_manager.h"

Entity::Entity(const char* name, EntityManager* entityManager) : name(name), is_active(true) 
{
    AddComponent<Transform>();
    entityManager->AddEntity(this);
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