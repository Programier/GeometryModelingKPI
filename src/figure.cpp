#include <figure.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <helpers.hpp>
#include <logger.hpp>
#include <point_transformer.hpp>
#include <properties.hpp>
#include <stdio.h>


Figure::Figure()
{
    _M_lines.emplace_back();// Main circle 0
    _M_lines.emplace_back();// second circle 1
    _M_lines.emplace_back();// line 2
    _M_lines.emplace_back();// second line 3
    _M_lines.emplace_back();// tangent 4
    _M_lines.emplace_back();// normal 5
}

Figure& Figure::instance()
{
    static Figure figure;
    return figure;
}

float Figure::calculate_k()
{
    return (properties.figure.R / properties.figure.r) - 1.0f;
}


void Figure::build_arc(Line& line, glm::vec2 center, float radius, float start_angle, float end_angle, int num_segments,
                       int start)
{
    float angle_step = (end_angle - start_angle) / static_cast<float>(num_segments);

    for (int i = start; i <= num_segments; ++i)
    {
        float angle = start_angle + i * angle_step;
        glm::vec2 p1(center.x + radius * cos(angle), center.y + radius * sin(angle));
        line.push_back(p1);
    }
}


float Figure::fx(float t)
{
    return properties.figure.r * k * (glm::cos(t) + (glm::cos(t * k) / k));
}

float Figure::fy(float t)
{
    return properties.figure.r * k * (glm::sin(t) - (glm::sin(t * k) / k));
}

#define wrap(x) [this](double t) { return x(t); }

float Figure::curvature_radius(float t)
{
    t = glm::radians(t);

    float dx  = derivative(wrap(fx), t, 1, 1e-5);
    float dy  = derivative(wrap(fy), t, 1, 1e-5);
    float dx2 = derivative(wrap(fx), t, 2, 1e-1, 1e-5);
    float dy2 = derivative(wrap(fy), t, 2, 1e-1, 1e-5);

    float a = glm::pow((dx * dx + dy * dy), 3.0 / 2.0);
    float b = dx * dy2 - dy * dx2;

    return glm::abs(a / b);
}

float Figure::square(float t0, float t1)
{
    auto dx = [this](float t) { return derivative([this](float t) { return fx(t); }, t, 1); };
    return glm::abs(
            integrate(glm::radians(t0), glm::radians(t1), [this, dx](float x) -> float { return fy(x) * dx(x); }));
}


void Figure::update_static_circle()
{
    Line& line = _M_lines[static_circle_index];

    if (line.empty() || !is_equal(c_R, properties.figure.R))
    {
        line.clear();
        build_arc(line, {0, 0}, properties.figure.R, 0, glm::radians(360.f), 180, 0);
    }
}

glm::vec2 Figure::get_point(float t)
{
    return {fx(t), fy(t)};
}

glm::vec2 Figure::get_tangent_point(float t0)
{

    glm::vec2 point = get_point(t0);

    float dx = derivative([this](double t) { return fx(t); }, t0);
    float dy = derivative([this](double t) { return fy(t); }, t0);

    float der = dy / dx;

    return {0.0, point.y + der * (-point.x)};
}


glm::vec2 Figure::get_normal_point(float t0)
{

    glm::vec2 point = get_point(t0);

    float dx = derivative([this](double t) { return fx(t); }, t0);
    float dy = derivative([this](double t) { return fy(t); }, t0);

    float der = dy / dx;

    return {0.0, point.y - (1 / der) * (-point.x)};
}


bool is_different_sing(double a, double b)
{
    return a <= 0.0 && b > 0 || a > 0 && b <= 0.0;
}

void Figure::get_bend_point(float start, float end, std::vector<glm::vec3>& out)
{
    out.clear();
    start = glm::radians(start);
    end   = glm::radians(end);

    float prev = -1;

    if (is_equal(start, 0.0))
    {
        auto res = get_point(start);
        out.push_back({res.x, res.y, glm::degrees(start)});
    }

    while (start < end)
    {
        float derivative = parametric_derivative([this](double t) { return fx(t); }, [this](double t) { return fy(t); },
                                                 start, 2, 1e-1, 1e-5);

        if (is_equal(derivative, 0.0))
        {
            auto res = get_point(start);

            out.push_back({res.x, res.y, glm::degrees(start)});
        }

        prev = derivative;

        start += 0.001;
    }
}


void Figure::update_dynamic_circle_and_radius()
{
    bool need_update = _M_lines[dynamic_circle_index].empty() || _M_lines[radius_index].empty() ||
                       !is_equal(c_R, properties.figure.R) || !is_equal(c_r, properties.figure.r) ||
                       !is_equal(c_a, current_angle);

    if (need_update)
    {
        float t = glm::radians(current_angle);

        Line* line = &_M_lines[dynamic_circle_index];

        line->clear();

        auto rotation_matrix = glm::rotate(glm::mat4(1.0), t, {0, 0, 1});
        glm::vec4 center4    = rotation_matrix * glm::vec4(properties.figure.R - properties.figure.r, 0.0, 0.0, 1.0);
        glm::vec2 center     = {center4.x, center4.y};
        build_arc(*line, center, properties.figure.r, 0, glm::radians(360.f), 180, 0);

        line = &_M_lines[radius_index];

        if (line->empty() && !_M_lines[curve_index].empty())
        {
            line->resize(2);
        }

        line->front() = center;
        line->back()  = _M_lines[curve_index].back();
    }
}

void Figure::update_curve()
{
    Line& line      = _M_lines[curve_index];
    const int limit = properties.figure.max_parts;

    if (!line.empty())
    {
        for (int i = 0; i < 3 && line.size() >= limit; i++) line.pop_front();
    }

    bool need_update = _M_lines[curve_index].empty() || !is_equal(c_R, properties.figure.R) ||
                       !is_equal(c_r, properties.figure.r) || !is_equal(c_a, current_angle);

    if (need_update)
    {
        float t = glm::radians(current_angle);
        line.push_back(get_point(t));
        return;
    }
}


void Figure::cache_data()
{
    c_R = properties.figure.R;
    c_r = properties.figure.r;
    c_a = current_angle;

    c_t0 = properties.figure.tangent_angle_0;
    c_n0 = properties.figure.normal_angle_0;
}

void Figure::update_tangent()
{
    if (!(_M_lines[tangent_index].empty() || !is_equal(c_t0, properties.figure.tangent_angle_0)))
    {
        return;
    }

    Line& line = _M_lines[tangent_index];

    if (line.size() != 2)
    {
        line.resize(2);
    }

    float t0 = glm::radians(properties.figure.tangent_angle_0);

    glm::vec2 start     = get_point(t0);
    glm::vec2 direction = glm::normalize(get_tangent_point(t0) - start);

    line.front() = start - direction;
    line.back()  = start + direction;
    return;
}

void Figure::update_normal()
{
    if (!(_M_lines[normal_index].empty() || !is_equal(c_n0, properties.figure.normal_angle_0)))
    {
        return;
    }

    Line& line = _M_lines[normal_index];

    if (line.size() != 2)
    {
        line.resize(2);
    }

    float t0 = glm::radians(properties.figure.normal_angle_0);

    glm::vec2 start     = get_point(t0);
    glm::vec2 direction = glm::normalize(get_normal_point(t0) - start);

    line.front() = start - direction;
    line.back()  = start + direction;
    return;
}

void Figure::update()
{
    if (!properties.figure.update)
        return;

    k = calculate_k();

    bool next_iteration = false;

    update_static_circle();


    if (properties.figure.disable_animation)
    {
        if ((c_a + properties.figure.angle_step < properties.figure.angle ||
             c_a - properties.figure.angle_step > properties.figure.angle || !is_equal(c_r, properties.figure.r) ||
             !is_equal(c_R, properties.figure.R) || _M_lines[curve_index].empty()))
        {
            // Calculate start angle
            c_a = properties.figure.angle -
                  properties.figure.angle_step * static_cast<float>(properties.figure.max_parts);
            c_a           = glm::max(c_a, 0.0f);
            current_angle = c_a;
            clear_curve();

            while (c_a + properties.figure.angle_step < properties.figure.angle)
            {
                update_curve();
                c_a = current_angle;
                current_angle += properties.figure.angle_step;
            }
        }
    }
    else
    {
        // Update current angle
        if (current_angle + properties.figure.angle_step < properties.figure.angle)
        {
            current_angle += properties.figure.angle_step;
        }
        else if (current_angle - properties.figure.angle_step > properties.figure.angle)
        {
            current_angle -= properties.figure.angle_step;
        }


        update_curve();
    }

    if (properties.figure.draw_tangent_at_current_point)
    {
        properties.figure.tangent_angle_0 = current_angle;
    }

    if (properties.figure.draw_normal_at_current_point)
    {
        properties.figure.normal_angle_0 = current_angle;
    }

    update_dynamic_circle_and_radius();
    update_tangent();
    update_normal();
    cache_data();
}


float Figure::get_current_angle()
{
    return current_angle;
}

void Figure::clear_curve()
{
    _M_lines[curve_index].clear();
}

void Figure::reset_angle()
{
    current_angle = 0.0f;
}


static float pow2(float a)
{
    return a * a;
}

float Figure::get_arc_len(float a, float b)
{
    return integrate(glm::radians(a), glm::radians(b), [this](float x) -> float {
        float a = derivative(wrap(fx), x, 1);
        float b = derivative(wrap(fy), x, 1);
        return glm::sqrt(a * a + b * b);
    });
}

void Figure::render(DrawFunc drawer)
{
    update();

    bool status[6] = {
            properties.figure.render_static_circle, properties.figure.render_dynamic_circle,
            properties.figure.render_curve,         properties.figure.render_radius,
            properties.figure.render_tangent,       properties.figure.render_normal,
    };

    int index = 0;
    for (const Line& line : _M_lines)
    {
        if (line.size() == 1)
            continue;

        if (status[index])
        {
            auto prev = line.begin();
            auto it   = ++line.begin();
            auto end  = line.end();

            while (it != end)
            {
                glm::vec2 p1 = PointTransformer::transform_with_euclidean(*prev);
                glm::vec2 p2 = PointTransformer::transform_with_euclidean(*it);
                drawer(p1, p2, properties.figure.color[index], properties.figure.color[index], true);
                prev = it;
                ++it;
            }
        }

        index++;
    }
}
