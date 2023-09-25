#pragma once
#include <glm/glm.hpp>
#include <vector>

class Figure
{
private:
    using Line = std::vector<glm::vec2>;
    std::vector<Line> _M_lines;

    Figure();

    void add_offset(glm::vec2);
    void build_arc(glm::vec2 center, float radius, float start_angle, float end_angle, int num_segments = 360);

public:
    using DrawFunc = void (*)(glm::vec2, glm::vec2, const glm::vec3&, const glm::vec3&, bool);

    static Figure& instance();

    bool build();
    void clear();
    void render(DrawFunc grid_space_drawer);
};
