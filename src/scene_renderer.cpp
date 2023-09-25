#include <definitions.hpp>
#include <properties.hpp>
#include <renderer.hpp>


static void draw_line(glm::ivec2 point1, glm::ivec2 point2, const glm::vec3& color1 = glm::vec3(1),
                      const glm::vec3& color2 = glm::vec3(1.0f), bool can_use_props_colors = true)
{
    Renderer::draw_line(point1, point2, color1, color2, can_use_props_colors);
}


static void draw_circle(glm::ivec2 center, int radius, int segments = 360)
{
    center += properties.grid.start_point;
    radius *= properties.grid.spacing;

    for (int i = 0; i < segments; i++)
    {
        float angle = 2 * M_PI * i / segments;
        draw_line({center.x + (int) (radius * cos(angle)), center.y + (int) (radius * sin(angle))},
                  {center.x + (int) (radius * cos(angle + 2 * M_PI / segments)),
                   center.y + (int) (radius * sin(angle + 2 * M_PI / segments))});
    }
}


void draw_grid_with_axes()
{
    float cache_line_width = properties.rendering.line_width;
    int width              = VIEWPORT_WIDTH;
    int height             = VIEWPORT_HEIGHT;
    int spacing            = properties.grid.spacing;

    glm::ivec2 origin = properties.grid.start_point;

    if (!properties.grid.disable_grid)
    {
        Renderer::line_width(1.0f);

        // Малюємо горизонтальні лінії сітки
        for (int y = origin.y; y <= height; y += spacing)
        {
            glm::ivec2 point1(0, y);
            glm::ivec2 point2(width, y);

            draw_line(point1, point2, properties.grid.h_color1, properties.grid.h_color2, false);
        }

        for (int y = origin.y - spacing; y > 0; y -= spacing)
        {
            glm::ivec2 point1(0, y);
            glm::ivec2 point2(width, y);
            draw_line(point1, point2, properties.grid.h_color1, properties.grid.h_color2, false);
        }

        // Малюємо вертикальні лінії сітки
        for (int x = origin.x; x <= width; x += spacing)
        {
            glm::ivec2 point1(x, 0);
            glm::ivec2 point2(x, height);
            draw_line(point1, point2, properties.grid.v_color1, properties.grid.v_color2, false);
        }

        for (int x = origin.x - spacing; x > 0; x -= spacing)
        {
            glm::ivec2 point1(x, 0);
            glm::ivec2 point2(x, height);
            draw_line(point1, point2, properties.grid.v_color1, properties.grid.v_color2, false);
        }
    }

    if (!properties.grid.disable_axis)
    {
        Renderer::line_width(2.0f);

        // Малюємо координатні вісі
        glm::ivec2 x_axis_start(0, origin.y);
        glm::ivec2 x_axis_end(width, origin.y);
        draw_line(x_axis_start, x_axis_end, glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), false);// Червона вісь X

        glm::ivec2 y_axis_start(origin.x, 0);
        glm::ivec2 y_axis_end(origin.x, height);
        draw_line(y_axis_start, y_axis_end, glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), false);// Зелена вісь Y
    }

    Renderer::line_width(cache_line_width);
}


void render_scene()
{
    draw_grid_with_axes();
    draw_circle({0, 0}, 2);
}
