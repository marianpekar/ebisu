#pragma once

class Component {
private:
    bool is_persistent = false;
public:
    class Entity* owner = nullptr;
    Component() = default;
    virtual ~Component() = default;
    virtual void Setup() {}
    virtual void Update(float delta_time) {}
    virtual void Render() {}
    bool GetIsPersistent() const { return is_persistent; }
    void SetIsPersistent(const bool persistent) { is_persistent = persistent; }
};