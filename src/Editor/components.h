#pragma once

struct Component
{
    Component() = default;
};

struct Transform : Component
{
    Transform(float x, float y) : x(x), y(y) {}
    float x;
    float y;
};