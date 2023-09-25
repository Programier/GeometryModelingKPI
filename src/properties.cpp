#include <SDL2/SDL_events.h>
#include <object.hpp>
#include <properties.hpp>


Properties::Properties()
{
    rendering.line_width   = 1.0f;
    rendering.point1_color = glm::vec3(1.0f);
    rendering.point2_color = glm::vec3(1.0f);

    grid.start_point = glm::ivec2(VIEWPORT_WIDTH / 2, VIEWPORT_HEIGHT / 2);
    grid.spacing     = DEFAULT_GRID_SPACING;
    grid.h_color1    = glm::vec3(0.5f);
    grid.h_color2    = glm::vec3(0.5f);
    grid.v_color1    = glm::vec3(0.5f);
    grid.v_color2    = glm::vec3(0.5f);

    grid.disable_axis = false;
    grid.disable_grid = false;
}

Properties properties;


struct PropertiesUpdater : public Object {

    bool right_mouse_button_pressed = false;

    virtual void process_event(SDL_Event* event)
    {
        if (event->type == SDL_MOUSEBUTTONDOWN)
        {
            if (event->button.button == SDL_BUTTON_RIGHT)
            {
                right_mouse_button_pressed = true;
            }
        }
        else if (event->type == SDL_MOUSEBUTTONUP)
        {
            if (event->button.button == SDL_BUTTON_RIGHT)
            {
                right_mouse_button_pressed = false;
            }
        }
        else if (event->type == SDL_MOUSEMOTION)
        {
            static glm::ivec2 prev_pos;

            glm::ivec2 current = glm::ivec2(event->motion.x, -event->motion.y);

            if (right_mouse_button_pressed)
            {
                glm::ivec2 diff = current - prev_pos;
                properties.grid.start_point += diff;
            }

            prev_pos = current;
        }
        else if (event->type == SDL_MOUSEWHEEL)
        {
            properties.grid.spacing =
                    glm::max(properties.grid.spacing + (event->wheel.y * glm::max(properties.grid.spacing / 10, 1)), 1);
        }
    }
} updater;
