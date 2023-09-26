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
    if (!properties.grid.disable_grid)
    {
        Renderer::line_width(1.0f);

        glm::vec2 start(-static_cast<float>(properties.grid.size.x), -static_cast<float>(properties.grid.size.y));
        glm::vec2 end(static_cast<float>(properties.grid.size.x), -static_cast<float>(properties.grid.size.y));

        for (glm::uint i = 0, j = properties.grid.size.y * 2; i <= j; ++i)
        {
            draw_line(PointTransformer::transform(start), PointTransformer::transform(end), properties.grid.h_color1,
                      properties.grid.h_color2, false);

            start.y += 1.0;
            end.y += 1.0;
        }

        start = glm::vec2(-static_cast<float>(properties.grid.size.x), -static_cast<float>(properties.grid.size.y));
        end   = glm::vec2(-static_cast<float>(properties.grid.size.x), static_cast<float>(properties.grid.size.y));

        for (glm::uint i = 0, j = properties.grid.size.x * 2; i <= j; ++i)
        {

            draw_line(PointTransformer::transform(start), PointTransformer::transform(end), properties.grid.v_color1,
                      properties.grid.v_color2, false);

            start.x += 1.0;
            end.x += 1.0;
        }
    }

    if (!properties.grid.disable_axis)
    {
        Renderer::line_width(2.0f);
        glm::vec2 start(-static_cast<float>(properties.grid.size.x), 0);
        glm::vec2 end(static_cast<float>(properties.grid.size.x), 0);

        static glm::vec3 x_color(1.0, 0.0, 0.0);
        static glm::vec3 y_color(0.0, 1.0, 0.0);
        draw_line(PointTransformer::transform(start), PointTransformer::transform(end), x_color, x_color, false);


        start.x = end.x = 0;
        start.y = end.y = static_cast<float>(properties.grid.size.y);
        start.y *= -1.0;
        draw_line(PointTransformer::transform(start), PointTransformer::transform(end), y_color, y_color, false);
    }

    Renderer::line_width(properties.rendering.line_width);
}


void render_scene()
{
    draw_grid_with_axes();

    if (properties.figure.render)
        Figure::instance().render(draw_line);
}
