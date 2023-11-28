#pragma once
#include <definitions.hpp>
#include <functional>
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
        glm::ivec4 size;
        glm::ivec2 start_point;
        glm::vec3 h_color1;
        glm::vec3 h_color2;
        glm::vec3 v_color1;
        glm::vec3 v_color2;

        std::int32_t spacing;

        bool disable_axis;
        bool disable_grid;
        bool enable_euclidean_transform;
    } grid;

    struct {
        float R;
        float r;
        float angle;
        float angle_step;
        int max_parts;

        glm::vec3 color[6];

        bool render_static_circle;
        bool render_dynamic_circle;
        bool render_curve;
        bool render_radius;

        float tangent_angle_0;
        bool draw_tangent_at_current_point;
        bool render_tangent;


        float normal_angle_0;
        bool draw_normal_at_current_point;
        bool render_normal;

        bool disable_animation;
        bool update;
    } figure;

    struct {
        struct {
            glm::vec2 offset;
            bool enable;
        } move;

        struct {
            glm::vec2 point;
            float angle;

            bool enable;
        } rotate;

        struct {
            glm::vec2 scale;
            bool enable;
        } scale;

        struct {
            bool XZ;
            bool YZ;
        } symmetry;
    } euclidean;


    struct {
        glm::vec2 R0;
        glm::vec2 Rx;
        glm::vec2 Ry;
        bool enable;
    } affine;

    struct {
        glm::vec2 R0;
        glm::vec2 Rx;
        glm::vec2 Ry;
        glm::vec2 W;
        float W0;
        bool enable;
    } projective;

    Properties();
    ~Properties();
};


extern Properties properties;
