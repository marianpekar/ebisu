#pragma once

#include <memory>

class Component
{
private:
    bool is_persistent = false;
public:
    std::shared_ptr<class Entity> owner;
    Component() = default;
    virtual ~Component() = default;
    virtual void Setup() {}
    virtual void Update(float delta_time) {}
    virtual void FixedUpdate(float fixed_delta_time) {}
    virtual void Render() {}
    virtual void Reset() {}
    bool GetIsPersistent() const { return is_persistent; }
    void SetIsPersistent(const bool persistent) { is_persistent = persistent; }
};