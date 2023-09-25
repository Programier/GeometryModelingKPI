#pragma once
#include <glm/glm.hpp>

struct Properties {
    struct {
        float line_width       = 1.0f;
        bool use_props_colors  = false;
        glm::vec4 point1_color = glm::vec4(1.0f);
        glm::vec4 point2_color = glm::vec4(1.0f);
    } rendering;
};


extern Properties properties;
