#pragma once
#include <glm/glm.hpp>

namespace Renderer
{
    void init();
    void terminate();

    void line_width(float width);
    void render_background();
    void draw_line(glm::vec2 point1, glm::vec2 point2, const glm::vec3& color1 = glm::vec3(1.0),
                   const glm::vec3& color2 = glm::vec3(1.0), bool can_use_props_colors = true);
}// namespace Renderer
