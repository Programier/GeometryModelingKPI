#pragma once
#include <definitions.hpp>
#include <functional>
#include <glm/glm.hpp>

struct Property {
    float value;

    virtual const char* name() = 0;
    virtual float max_value()  = 0;
    virtual float min_value()  = 0;

    virtual ~Property();
};


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

        union
        {
            struct {
                Property* A;
                Property* B;
                Property* C;
                Property* D;
                Property* E;
                Property* F;
                Property* G;
                Property* H;
            };

            Property* props[FIGURE_PROPS_COUNT];
        };

        glm::vec3 color;


        glm::vec2 offset;

        struct {
            glm::vec2 point;
            float angle;
        } rotate;


        bool render;

    } figure;


    struct {
        glm::vec2 R0;
        glm::vec2 Rx;
        glm::vec2 Ry;
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

    float figure_prop(std::size_t index) const;
    ~Properties();
};


extern Properties properties;
