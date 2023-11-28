#include <definitions.hpp>
#include <figure.hpp>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl2.h>
#include <opengl.hpp>
#include <properties.hpp>
#include <window.hpp>


struct Indent {
    Indent()
    {
        ImGui::Indent(ITEM_INDEND);
    }

    ~Indent()
    {
        ImGui::Unindent(ITEM_INDEND);
    }
};

static inline void push_empty_line(bool with_separator = true)
{
    ImGui::Spacing();
    if (with_separator)
        ImGui::Separator();
    ImGui::Spacing();
}


static void rendering_properties()
{
    if (ImGui::CollapsingHeader("Rendering"))
    {
        Indent indent;
        if (ImGui::SliderFloat("Line Width", &properties.rendering.line_width, 0.1f, 10.f))
        {
            glLineWidth(properties.rendering.line_width);
        }

        ImGui::Checkbox("Use properties colors", &properties.rendering.use_props_colors);
        ImGui::SliderFloat3("Point1 color", &properties.rendering.point1_color.x, 0.0, 1.0);
        ImGui::SliderFloat3("Point2 color", &properties.rendering.point2_color.x, 0.0, 1.0);

        if (ImGui::CollapsingHeader("Background"))
        {
            Indent indent;
            ImGui::SliderFloat3("ld color", &properties.rendering.ld_color.x, 0.0, 1.0);
            ImGui::SliderFloat3("lu color", &properties.rendering.lu_color.x, 0.0, 1.0);
            ImGui::SliderFloat3("rd color", &properties.rendering.rd_color.x, 0.0, 1.0);
            ImGui::SliderFloat3("ru color", &properties.rendering.ru_color.x, 0.0, 1.0);
        }
    }
}

static void grid_properties()
{
    if (ImGui::CollapsingHeader("Grid"))
    {
        Indent indent;
        if (ImGui::InputInt4("Size", &properties.grid.size.x))
        {
            properties.grid.size = glm::abs(properties.grid.size);
        }

        ImGui::SliderInt2("Start Pos", &properties.grid.start_point.x, -10000, 10000);
        ImGui::SliderInt("Spacing", &properties.grid.spacing, 1, 1000);

        if (ImGui::CollapsingHeader("Vertical Lines"))
        {
            Indent indent;
            ImGui::SliderFloat3("V. Color1", &properties.grid.v_color1.x, 0.0, 1.0);
            ImGui::SliderFloat3("V. Color2", &properties.grid.v_color2.x, 0.0, 1.0);
        }

        if (ImGui::CollapsingHeader("Horizontal Lines"))
        {
            Indent indent;
            ImGui::SliderFloat3("H. Color1", &properties.grid.h_color1.x, 0.0, 1.0);
            ImGui::SliderFloat3("H. Color2", &properties.grid.h_color2.x, 0.0, 1.0);
        }

        ImGui::Checkbox("Disable Grid", &properties.grid.disable_grid);
        ImGui::Checkbox("Disable Axis", &properties.grid.disable_axis);
        ImGui::Checkbox("Enable Euclidean transform", &properties.grid.enable_euclidean_transform);
    }
}

static void figure_properties()
{
    if (ImGui::CollapsingHeader("Figure"))
    {
        Indent indent;

        auto& p = properties.figure;

        if (ImGui::DragFloat("R", &p.R, .1f, p.r, 20.f))
        {
            Figure::instance().clear_curve();
        }

        if (ImGui::DragFloat("r", &p.r, .1f, 0, p.R))
        {
            Figure::instance().clear_curve();
        }

        if (ImGui::DragFloat("Angle##1", &p.angle, 1.0f, 0.0f, FLT_MAX))
        {
            p.angle = glm::max(0.0f, p.angle);
        }

        ImGui::Text("Current Angle: %f", Figure::instance().get_current_angle());
        ImGui::SliderFloat("Angle Step", &p.angle_step, 0, 10, "%.3f");

        ImGui::DragInt("Max Parts", &p.max_parts, 1.0f, 10, 2048);


        if (ImGui::CollapsingHeader("Colors"))
        {
            Indent indent;
            static const char* names[6] = {
                    "Static Circle Color", "Dynamic Circle Color", "Curve Color",
                    "Radius Color",        "Tangent Color",        "Normal Color",
            };

            for (int i = 0; i < 6; i++)
            {
                ImGui::ColorPicker3(names[i], &properties.figure.color[i].x);
            }
        }
        push_empty_line(false);


        if (ImGui::CollapsingHeader("Tangent"))
        {
            Indent indent;
            ImGui::DragFloat("t0##1", &p.tangent_angle_0, .1, 0.0, FLT_MAX);
            ImGui::Checkbox("Render Tangent", &properties.figure.render_tangent);
            ImGui::Checkbox("Render Tangent At Current Point", &properties.figure.draw_tangent_at_current_point);
        }

        if (ImGui::CollapsingHeader("Normal"))
        {
            Indent indent;
            ImGui::DragFloat("t0##2", &p.normal_angle_0, .1, 0.0, FLT_MAX);
            ImGui::Checkbox("Render Normal", &properties.figure.render_normal);
            ImGui::Checkbox("Render Normal At Current Point", &properties.figure.draw_normal_at_current_point);
        }

        push_empty_line(false);

        ImGui::Checkbox("Render Static Circle", &properties.figure.render_static_circle);
        ImGui::Checkbox("Render Dynamic Circle", &properties.figure.render_dynamic_circle);
        ImGui::Checkbox("Render Curve", &properties.figure.render_curve);
        ImGui::Checkbox("Render Radius", &properties.figure.render_radius);

        ImGui::Checkbox("Update Figure", &properties.figure.update);
        ImGui::Checkbox("Disable animation", &properties.figure.disable_animation);

        ImGui::Separator();
        if (ImGui::Button("Clear Curve"))
        {
            Figure::instance().clear_curve();
        }

        if (ImGui::Button("Reset Current Angle"))
        {
            Figure::instance().reset_angle();
            Figure::instance().clear_curve();
        }
    }
}

static void info_properties()
{
    if (ImGui::CollapsingHeader("Info"))
    {
        Indent indent;


        if (ImGui::CollapsingHeader("Arc Len"))
        {
            Indent indent;
            static float a = 0;
            static float b = 0;
            ImGui::DragFloat("Start##ArcLen", &a, 0.1, 0.0, b);
            ImGui::DragFloat("End##ArcLen", &b, 0.1, a, FLT_MAX);

            ImGui::Text("Len: %f", Figure::instance().get_arc_len(a, b));
        }

        if (ImGui::CollapsingHeader("Bend Points"))
        {
            Indent indent;
            static float a = 0.0;
            static float b = 0.0;
            ImGui::DragFloat("Start##Bend", &a, 0.1, 0.0, b);
            ImGui::DragFloat("End##Bend", &b, 0.1, a, FLT_MAX);

            if (ImGui::CollapsingHeader("Result"))
            {
                static std::vector<glm::vec3> points;
                Figure::instance().get_bend_point(a, b, points);

                for (auto& point : points)
                {
                    ImGui::Text("{%f : %f} -> angle = %f", point.x, point.y, point.z);
                }
            }
        }

        if (ImGui::CollapsingHeader("Radius of curvature"))
        {
            Indent indent;
            static float angle;
            ImGui::DragFloat("Angle##Radius of curvature", &angle, 0.1, 0., FLT_MAX);
            ImGui::Text("Radius: %f", Figure::instance().curvature_radius(angle));
        }

        if (ImGui::CollapsingHeader("Square"))
        {
            Indent indent;
            static float a = 0.0;
            static float b = 0.0;
            ImGui::DragFloat("Start##Square", &a, 0.1, 0.0, b);
            ImGui::DragFloat("End##Square", &b, 0.1, a, FLT_MAX);
            ImGui::Text("Square: %f", Figure::instance().square(a, b));
        }
    }
}


static void euclidean_properties()
{
    if (ImGui::CollapsingHeader("Euclidean"))
    {
        Indent indent;
        if (ImGui::CollapsingHeader("Move"))
        {
            Indent indent;
            ImGui::InputFloat2("Offset", &properties.euclidean.move.offset.x);
            ImGui::Checkbox("Enable##1", &properties.euclidean.move.enable);
        }

        push_empty_line(false);

        if (ImGui::CollapsingHeader("Rotate"))
        {
            Indent indent;
            ImGui::InputFloat2("Point##1", &properties.euclidean.rotate.point.x);
            ImGui::InputFloat("Angle##2", &properties.euclidean.rotate.angle);
            ImGui::Checkbox("Enable##2", &properties.euclidean.rotate.enable);
        }

        push_empty_line(false);

        if (ImGui::CollapsingHeader("Scale"))
        {
            Indent indent;
            ImGui::InputFloat2("Scale##1", &properties.euclidean.scale.scale.x);
            ImGui::Checkbox("Enable##3", &properties.euclidean.scale.enable);
        }

        push_empty_line(false);

        if (ImGui::CollapsingHeader("Symmetry"))
        {
            Indent indent;
            ImGui::Checkbox("XZ", &properties.euclidean.symmetry.XZ);
            ImGui::Checkbox("YZ", &properties.euclidean.symmetry.YZ);
        }
    }
}

static void reset_properties()
{
    std::destroy_at(&properties);
    new (&properties) Properties();
}

static void affine_properties()
{
    if (ImGui::CollapsingHeader("Affine"))
    {
        Indent indent;
        ImGui::InputFloat2("R0##1", &properties.affine.R0.x);
        ImGui::InputFloat2("Rx##1", &properties.affine.Rx.x);
        ImGui::InputFloat2("Ry##1", &properties.affine.Ry.x);
        ImGui::Checkbox("Enable##4", &properties.affine.enable);
    }
}

static void projective_properties()
{
    if (ImGui::CollapsingHeader("Projective"))
    {
        Indent indent;
        ImGui::InputFloat2("R0##2", &properties.projective.R0.x);
        ImGui::InputFloat2("Rx##2", &properties.projective.Rx.x);
        ImGui::InputFloat2("Ry##2", &properties.projective.Ry.x);
        ImGui::InputFloat2("W##2", &properties.projective.W.x);
        ImGui::InputFloat("W0##2", &properties.projective.W0);
        ImGui::Checkbox("Enable##5", &properties.projective.enable);
    }
}

void Window::render_ui()
{
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(_M_window);
    ImGui::NewFrame();

    ImGui::Begin("Properties", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    if (_M_is_first_frame)
    {
        ImGui::SetWindowPos({0, 0});
        ImGui::SetWindowSize({PROPERTIES_WIDTH, WINDOW_HEIGHT});
    }

    rendering_properties();
    push_empty_line();
    grid_properties();
    push_empty_line();
    figure_properties();
    push_empty_line();
    info_properties();
    push_empty_line();
    euclidean_properties();
    push_empty_line();
    affine_properties();
    push_empty_line();
    projective_properties();

    if (ImGui::Button("Reset"))
    {
        reset_properties();
    }

    ImGui::End();
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
