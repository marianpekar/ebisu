#pragma once

class Component {
public:
    class Entity* owner = nullptr;
    class Transform* transform = nullptr;
    Component() = default;
    virtual ~Component() = default;
    virtual void Update(float delta_time) {}
    virtual void Render() {}
};