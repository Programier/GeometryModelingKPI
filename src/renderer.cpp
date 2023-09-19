#include <SDL2/SDL.h>
#include <renderer.hpp>
#include <window.hpp>


namespace Renderer
{
    static SDL_Window* window;
    static SDL_Renderer* renderer;

    void init()
    {
        window   = Window::instance()->window();
        renderer = Window::instance()->renderer();
    }

    static glm::ivec2 apply_scale(glm::ivec2 point, float scale)
    {
        point.x = static_cast<int>(static_cast<float>(point.x) / scale);
        point.y = static_cast<int>(static_cast<float>(point.y) / scale);
        return point;
    }

    void draw_line(glm::ivec2 point1, glm::ivec2 point2, const glm::uvec4& color, float line_width)
    {
        point1 = apply_scale(point1, line_width);
        point2 = apply_scale(point2, line_width);


        SDL_RenderSetScale(renderer, line_width, line_width);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(renderer, point1.x, point1.y, point2.x, point2.y);
        SDL_RenderSetScale(renderer, 1.0f, 1.0f);
    }
}// namespace Renderer
