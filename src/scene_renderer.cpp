#include <definitions.hpp>
#include <figure.hpp>
#include <helpers.hpp>
#include <point_transformer.hpp>
#include <properties.hpp>
#include <renderer.hpp>


static void draw_line(glm::vec2 point1, glm::vec2 point2, const glm::vec3& color1 = glm::vec3(1),
                      const glm::vec3& color2 = glm::vec3(1.0f), bool can_use_props_colors = true)
{
    point1 = to_screen_space(point1);
    point2 = to_screen_space(point2);

    Renderer::draw_line(point1, point2, color1, color2, can_use_props_colors);
}


void draw_grid_with_axes()
{
    float cache_line_width = properties.rendering.line_width;
    float width            = VIEWPORT_WIDTH;
    float height           = VIEWPORT_HEIGHT;
    float spacing          = static_cast<float>(properties.grid.spacing);

    glm::vec2 origin = vec2_cast<glm::vec2>(properties.grid.start_point);

    if (!properties.grid.disable_grid)
    {
        Renderer::line_width(1.0f);

        // Малюємо горизонтальні лінії сітки
        for (float y = origin.y; y <= height; y += spacing)
        {

            glm::vec2 point1 = PointTransformer::transform(to_grid_space(0, y));
            glm::vec2 point2 = PointTransformer::transform(to_grid_space(width, y));
            draw_line(point1, point2, properties.grid.h_color1, properties.grid.h_color2, false);
        }

        for (int y = origin.y - spacing; y > 0; y -= spacing)
        {
            glm::vec2 point1 = PointTransformer::transform(to_grid_space(0, y));
            glm::vec2 point2 = PointTransformer::transform(to_grid_space(width, y));
            draw_line(point1, point2, properties.grid.h_color1, properties.grid.h_color2, false);
        }

        // Малюємо вертикальні лінії сітки
        for (float x = origin.x; x <= width; x += spacing)
        {
            glm::vec2 point1 = PointTransformer::transform(to_grid_space(x, 0));
            glm::vec2 point2 = PointTransformer::transform(to_grid_space(x, height));
            draw_line(point1, point2, properties.grid.v_color1, properties.grid.v_color2, false);
        }

        for (float x = origin.x - spacing; x > 0; x -= spacing)
        {
            glm::vec2 point1 = PointTransformer::transform(to_grid_space(x, 0));
            glm::vec2 point2 = PointTransformer::transform(to_grid_space(x, height));
            draw_line(point1, point2, properties.grid.v_color1, properties.grid.v_color2, false);
        }
    }

    if (!properties.grid.disable_axis)
    {
        Renderer::line_width(2.0f);


        // Малюємо координатні вісі
        glm::vec2 x_axis_start = to_grid_space(0, origin.y);
        x_axis_start           = PointTransformer::transform(x_axis_start);
        glm::vec2 x_axis_end   = PointTransformer::transform(to_grid_space(width, origin.y));
        draw_line(x_axis_start, x_axis_end, glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), false);// Червона вісь X

        //        glm::vec2 y_axis_start = PointTransformer::transform(to_grid_space(origin.x, 0));
        //        glm::vec2 y_axis_end   = PointTransformer::transform(to_grid_space(origin.x, height));
        //        draw_line(y_axis_start, y_axis_end, glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), false);// Зелена вісь Y
    }

    Renderer::line_width(cache_line_width);
}


void render_scene()
{
    draw_grid_with_axes();

    if (properties.figure.render)
        Figure::instance().render(draw_line);
}
