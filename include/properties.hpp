#pragma once
#include <definitions.hpp>
#include <glm/glm.hpp>

struct Properties {
    struct {
        float line_width;
        bool use_props_colors;
        glm::vec3 point1_color;
        glm::vec3 point2_color;

        glm::vec3 ld_color;
        glm::vec3 lu_color;
        glm::vec3 rd_color;
        glm::vec3 ru_color;
    } rendering;

    struct {
        glm::ivec2 start_point;
        std::int32_t spacing;
        glm::vec3 h_color1;
        glm::vec3 h_color2;
        glm::vec3 v_color1;
        glm::vec3 v_color2;

        bool disable_axis;
        bool disable_grid;
    } grid;

    struct {

    } object;

    Properties();
};


extern Properties properties;
