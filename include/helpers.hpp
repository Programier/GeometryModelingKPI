#pragma once

#include <glm/glm.hpp>
#include <stdexcept>

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

bool is_equal(float a, float b);


template<typename Func, typename... Args>
double derivative(Func func, double x, size_t order = 1, double h = 1e-5, Args... args)
{
    if (order == 0)
        return func(x);

    double y1 = derivative(func, x, order - 1, args...);
    double y2 = derivative(func, x + h, order - 1, args...);

    return (y2 - y1) / (h);
}

template<typename Func, typename Func2>
double parametric_derivative(Func fx, Func2 fy, double t, size_t order = 1, double h2 = 1e-1, double h1 = 1e-5)
{
    if (order == 1)
        return fy(t) / fx(t);

    if (order == 2)
    {
        double dx_dt   = derivative(fx, t, 1, h1);
        double dy_dt   = derivative(fy, t, 1, h1);
        double d2x_dt2 = derivative(fx, t, 2, h2, h1);
        double d2y_dt2 = derivative(fy, t, 2, h2, h1);
        return (d2y_dt2 - (d2x_dt2 * dy_dt) / dx_dt) / (dx_dt * dx_dt);
    }

    throw std::runtime_error("Unsupported order!");
}

template<typename Func>
static float integrate(float a, float b, Func f, float step = 0.01)
{
    double sum = 0.0;

    while (a < b)
    {
        sum += f(a);
        a += step;
    }

    return step * sum;
}
