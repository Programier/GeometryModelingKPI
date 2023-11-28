#include <SDL2/SDL_events.h>
#include <figure.hpp>
#include <imgui/imgui.h>
#include <iostream>
#include <logger.hpp>
#include <object.hpp>
#include <properties.hpp>


Properties::Properties()
{
    rendering.line_width   = 6.0f;
    rendering.point1_color = glm::vec3(1.0f);
    rendering.point2_color = glm::vec3(1.0f);

    grid.start_point = glm::ivec2(VIEWPORT_WIDTH / 2, VIEWPORT_HEIGHT / 2);
    grid.spacing     = DEFAULT_GRID_SPACING;
    grid.h_color1    = glm::vec3(0.5f);
    grid.h_color2    = glm::vec3(0.5f);
    grid.v_color1    = glm::vec3(0.5f);
    grid.v_color2    = glm::vec3(0.5f);

    grid.disable_axis               = false;
    grid.disable_grid               = false;
    grid.size                       = {20, 20, 20, 20};
    grid.enable_euclidean_transform = false;

    euclidean.scale.scale = {1, 1};
    euclidean.move.offset = {0, 0};

    euclidean.move.enable   = true;
    euclidean.rotate.enable = true;
    euclidean.scale.enable  = true;
    affine.enable           = true;


    figure.R          = PROP_R / GRID_UNIT_SEGMENT;
    figure.r          = PROP_r / GRID_UNIT_SEGMENT;
    figure.angle      = 0;
    figure.angle_step = PROP_delta_ange;
    figure.max_parts  = PROP_max_parts;


    figure.color[0] = glm::vec3(1.0, 1.0, 0.0);
    figure.color[1] = glm::vec3(0.0, 1.0, 0.0);
    figure.color[2] = glm::vec3(0.0, 0.0, 1.0);
    figure.color[3] = glm::vec3(1.0, 0.0, 0.0);
    figure.color[4] = glm::vec3(1.0, 0.0, 1.0);
    figure.color[5] = glm::vec3(1.0, 1.0, 1.0);

    figure.update = true;

    affine.Rx = glm::vec2(1.0, 0.0);
    affine.Ry = glm::vec2(0.0, 1.0);


    figure.tangent_angle_0               = 0.0;
    figure.draw_tangent_at_current_point = false;
    figure.normal_angle_0                = 0.0;
    figure.draw_normal_at_current_point  = false;

    figure.render_static_circle  = true;
    figure.render_dynamic_circle = true;
    figure.render_curve          = true;
    figure.render_radius         = true;
    figure.render_tangent        = true;
    figure.render_normal         = true;

    figure.update            = true;
    figure.disable_animation = false;


    projective.R0     = {0, 0};
    projective.Rx     = {150, 0};
    projective.Ry     = {0, 50.f};
    projective.W      = {1.f, 30.f};
    projective.W0     = 100.f;
    projective.enable = false;
}

Properties::~Properties()
{}

Properties properties;


struct PropertiesUpdater : public Object {

    bool left_mouse_button_pressed = false;
    bool can_scroll                = false;

    static bool point_in_viewport(glm::uvec2 point)
    {
        if (point.x > WINDOW_WIDTH || point.x < PROPERTIES_WIDTH)
            return false;
        return true;
    }

    virtual void process_event(SDL_Event* event)
    {
        static glm::ivec2 prev_pos;

        if (event->type == SDL_MOUSEBUTTONDOWN)
        {
            if (event->button.button == SDL_BUTTON_LEFT && point_in_viewport(prev_pos))
            {
                left_mouse_button_pressed = true;
            }
        }
        else if (event->type == SDL_MOUSEBUTTONUP)
        {
            if (event->button.button == SDL_BUTTON_LEFT)
            {
                left_mouse_button_pressed = false;
            }
        }
        else if (event->type == SDL_MOUSEMOTION)
        {
            glm::ivec2 current = glm::ivec2(event->motion.x, -event->motion.y);

            if (left_mouse_button_pressed)
            {
                glm::ivec2 diff = current - prev_pos;
                properties.grid.start_point += diff;
            }

            prev_pos   = current;
            can_scroll = point_in_viewport(current);
        }
        else if (can_scroll && event->type == SDL_MOUSEWHEEL)
        {
            properties.grid.spacing =
                    glm::max(properties.grid.spacing + (event->wheel.y * glm::max(properties.grid.spacing / 10, 1)), 1);
        }
    }
} updater;
