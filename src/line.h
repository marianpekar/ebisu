#pragma once

#include "Math/vector2.h"

class Line
{
    const float vertical_gradient = 1e6f;

    float gradient;
    float y_intercept;
    Vector2 point_a;
    Vector2 point_b;
    bool approach_side;

    bool GetSide(const Vector2& point) const;
    bool HasCrossedLine(const Vector2& point) const;
public:
    Line(const Vector2& point, const Vector2& point_perpendicular);
};
