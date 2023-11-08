#include "line.h"

Line::Line(const Vector2& point, const Vector2& point_perpendicular)
{
    const float dx = point.x - point_perpendicular.x;
    const float dy = point.y - point_perpendicular.y;
        
    if (std::abs(dy) < 1e-6f)
    {
        gradient = vertical_gradient;
    }
    else
    {
        gradient = -dx / dy;
    }

    y_intercept = point.y - gradient * point.x;
    point_a = point;
    point_b = point + Vector2(1, gradient);

    approach_side = GetSide(point_perpendicular);
}

bool Line::GetSide(const Vector2& point) const
{
    return (point.x - point_a.x) * (point_b.y - point_a.y) > (point.y - point_a.y) * (point_b.x - point_a.x);
}

bool Line::HasCrossedLine(const Vector2& point) const
{
    return GetSide(point) != approach_side;
}
