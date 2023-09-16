#pragma once

#include <cmath>

struct Vector2 
{
    float x, y;

    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(const float x, const float y) : x(x), y(y) {}

    Vector2 operator+(const Vector2& other) const 
    {
        return { x + other.x, y + other.y };
    }

    Vector2 operator-(const Vector2& other) const 
    {
        return { x - other.x, y - other.y };
    }

    Vector2 operator-() const
    {
        return { -x, -y };
    }

    Vector2 operator*(const float scalar) const 
    {
        return { x * scalar, y * scalar };
    }

    Vector2 operator/(const float scalar) const 
    {
        return { x / scalar, y / scalar };
    }

    constexpr Vector2& operator+=(const Vector2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr Vector2& operator+(const float& scalar)
    {
        x += scalar;
        y += scalar;
        return *this;
    }

    constexpr Vector2& operator-=(const Vector2& other) 
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    constexpr Vector2& operator*=(const float scalar) 
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    constexpr Vector2& operator/=(const float scalar) 
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    float Dot(const Vector2& other) const 
    {
        return x * other.x + y * other.y;
    }


    float Length() const 
    {
        return std::sqrt(x * x + y * y);
    }

    Vector2 Normalized() const 
    {
        if (const float len = Length(); len != 0.0f) {
            return *this / len;
        }
        
        return { 0.0f, 0.0f };
    }
};
