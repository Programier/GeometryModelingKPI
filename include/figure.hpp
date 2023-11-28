#pragma once
#include <glm/glm.hpp>
#include <list>
#include <vector>

static constexpr inline int static_circle_index  = 0;
static constexpr inline int dynamic_circle_index = 1;
static constexpr inline int curve_index          = 2;
static constexpr inline int radius_index         = 3;
static constexpr inline int tangent_index        = 4;
static constexpr inline int normal_index         = 5;

class Figure
{
private:
    using Line = std::list<glm::vec2>;
    std::vector<Line> _M_lines;

    Figure();
    float k;

    float c_R;
    float c_r;
    float c_a;
    float current_angle;

    float c_t0;
    float c_n0;

    static float calculate_k();
    void build_arc(Line& line, glm::vec2 center, float radius, float start_angle, float end_angle, int num_segments,
                   int start);

public:
    using DrawFunc = void (*)(glm::vec2, glm::vec2, const glm::vec3&, const glm::vec3&, bool);

    static Figure& instance();


    void update_static_circle();
    void update_curve();
    void update_dynamic_circle_and_radius();
    void update_tangent();
    void update_normal();

    void cache_data();

    void update();

    float fx(float t);
    float fy(float t);

    float curvature_radius(float t);
    float square(float t0, float t1);


    glm::vec2 get_point(float t);
    glm::vec2 get_tangent_point(float t);
    glm::vec2 get_normal_point(float t);
    void get_bend_point(float start, float end, std::vector<glm::vec3>& out);

    float get_arc_len(float a, float b);

    float get_current_angle();
    void clear_curve();
    void reset_angle();

    void render(DrawFunc grid_space_drawer);
};
