#include <helpers.hpp>
#include <properties.hpp>


glm::vec2 to_grid_space(glm::vec2 screen_space)
{
    return (screen_space - vec2_cast<glm::vec2>(properties.grid.start_point)) /
           static_cast<float>(properties.grid.spacing);
}

glm::vec2 to_screen_space(glm::vec2 grid_space)
{
    return (grid_space * static_cast<float>(properties.grid.spacing)) +
           vec2_cast<glm::vec2>(properties.grid.start_point);
}

glm::vec2 to_grid_space(float x, float y)
{
    return to_grid_space({x, y});
}

glm::vec2 to_screen_space(float x, float y)
{
    return to_screen_space({x, y});
}

bool is_equal(float a, float b)
{
    return a > b - ACCURACY && a < b + ACCURACY;
}
