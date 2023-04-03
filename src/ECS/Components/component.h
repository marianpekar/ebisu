#pragma once

class Entity;
class Component {
public:
    Entity* owner;
    Component() : owner(nullptr) {}
    virtual ~Component() = default;
    virtual void Update(float delta_time) {}
    virtual void Render() {}
};