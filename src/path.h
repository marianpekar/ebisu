#pragma once

#include <utility>
#include <vector>
#include "line.h"
#include "Math/vector2.h"

class Path
{
public:
    std::vector<Vector2> path;
    std::vector<Line*> turn_boundaries;
    size_t end_line_index;

    Path(std::vector<Vector2> path, const Vector2 start_pos, const float turn_distance)
    {
        this->path = std::move(path);
        end_line_index = this->path.size() - 1;

        Vector2 previous_point = start_pos;
        for (size_t i = 0; i < this->path.size(); i++)
        {
            Vector2 current_point = this->path[i];
            Vector2 dir_to_current_point = (current_point - previous_point).Normalized();
            Vector2 turn_boundary_point = (i == end_line_index) ? current_point : current_point - dir_to_current_point * turn_distance;
            auto line = new Line(turn_boundary_point, previous_point - dir_to_current_point * turn_distance);
            turn_boundaries.emplace_back(line);
            previous_point = turn_boundary_point;
        }
    }

    ~Path()
    {
        for (const auto line : turn_boundaries)
        {
            delete line;
        }
    }
};
