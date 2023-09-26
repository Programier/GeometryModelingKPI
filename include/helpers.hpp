#pragma once

#include <glm/glm.hpp>

template<typename Out, typename In>
inline Out vec2_cast(In in)
{
    Out out;
    out.x = static_cast<Out::value_type>(in.x);
    out.y = static_cast<Out::value_type>(in.y);
    return out;
}

glm::vec2 to_grid_space(glm::vec2 screen_space);
glm::vec2 to_screen_space(glm::vec2 grid_space);

glm::vec2 to_grid_space(float x, float y);
glm::vec2 to_screen_space(float x, float y);


