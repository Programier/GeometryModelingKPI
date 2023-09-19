#include <renderer.hpp>

static void draw_circle(glm::ivec2 center, int radius, int segments = 360)
{
    for (int i = 0; i < segments; i++)
    {
        float angle = 2 * M_PI * i / segments;
        Renderer::draw_line({center.x + (int) (radius * cos(angle)), center.y + (int) (radius * sin(angle))},
                            {center.x + (int) (radius * cos(angle + 2 * M_PI / segments)),
                             center.y + (int) (radius * sin(angle + 2 * M_PI / segments))});
    }
}

void render_scene()
{
    draw_circle({400, 300}, 150, 360);
}
