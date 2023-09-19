#pragma once
#include <glm/glm.hpp>

namespace Renderer
{
    void init();
    void draw_line(glm::ivec2 point1, glm::ivec2 point2, const glm::uvec4& color = glm::vec4(255),
                   float line_width = 1.0f);
}// namespace Renderer
