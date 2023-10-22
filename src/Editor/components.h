#pragma once

class Component
{
public:
    Component(const char* name) : name(name) {}
    const char* GetName() const { return name; }

private:
    const char* name;
};

template<typename T>
class ComponentProperty
{
public:
    ComponentProperty() : name(nullptr), value(T()) {}
    ComponentProperty(const char* name, T value) : name(name), value(value) {}
    T& GetValue() const { return value; }
    void SetValue(T& value) { this->value = value; }
    
private:
    const char* name;
    T value;
};

struct Transform : Component
{
    Transform(const float x, const float y) : Component("Transform")
    {
        this->x = ComponentProperty("X", x);
        this->y = ComponentProperty("Y", y);
    }
    
    ComponentProperty<float> x;
    ComponentProperty<float> y;
};


