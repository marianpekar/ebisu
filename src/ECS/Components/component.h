#pragma once

class Component {
public:
    class Entity* owner = nullptr;
    Component() = default;
    virtual ~Component() = default;
    virtual void Setup() {}
    virtual void Update(float delta_time) {}
    virtual void Render() {}
};