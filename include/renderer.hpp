#pragma once
#include <glm/glm.hpp>

namespace Renderer
{
    void init();
    void terminate();

    void render_background();
    void draw_line(glm::ivec2 point1, glm::ivec2 point2, const glm::vec4& color1 = glm::vec4(1.0),
                   const glm::vec4& color2 = glm::vec4(1.0));
}// namespace Renderer
