#include <definitions.hpp>
#include <figure.hpp>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl2.h>
#include <opengl.hpp>
#include <properties.hpp>
#include <window.hpp>


static inline void push_empty_line()
{
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
}


static void rendering_properties()
{
    if (ImGui::CollapsingHeader("Rendering"))
    {
        if (ImGui::SliderFloat("Line Width", &properties.rendering.line_width, 0.1f, 5.f))
        {
            glLineWidth(properties.rendering.line_width);
        }

        ImGui::Checkbox("Use properties colors", &properties.rendering.use_props_colors);
        ImGui::SliderFloat3("Point1 color", &properties.rendering.point1_color.x, 0.0, 1.0);
        ImGui::SliderFloat3("Point2 color", &properties.rendering.point2_color.x, 0.0, 1.0);

        if (ImGui::CollapsingHeader("Background"))
        {
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
        ImGui::SliderInt2("Start Pos", &properties.grid.start_point.x, -10000, 10000);
        ImGui::SliderInt("Spacing", &properties.grid.spacing, 1, 1000);

        if (ImGui::CollapsingHeader("Vertical Lines"))
        {
            ImGui::SliderFloat3("V. Color1", &properties.grid.v_color1.x, 0.0, 1.0);
            ImGui::SliderFloat3("V. Color2", &properties.grid.v_color2.x, 0.0, 1.0);
        }

        if (ImGui::CollapsingHeader("Horizontal Lines"))
        {
            ImGui::SliderFloat3("H. Color1", &properties.grid.h_color1.x, 0.0, 1.0);
            ImGui::SliderFloat3("H. Color2", &properties.grid.h_color2.x, 0.0, 1.0);
        }

        ImGui::Checkbox("Disable Grid", &properties.grid.disable_grid);
        ImGui::Checkbox("Disable Axis", &properties.grid.disable_axis);
    }
}

static void figure_properties()
{
    if (ImGui::CollapsingHeader("Figure"))
    {
        if (ImGui::CollapsingHeader("Properties"))
        {
            for (Property* prop : properties.figure.props)
            {
                if (ImGui::SliderFloat(prop->name(), &prop->value, prop->min_value(), prop->max_value()))
                {
                    Figure::instance().build();
                }
            }

            ImGui::SliderFloat3("Figure Color", &properties.figure.color.x, 0.0, 1.0f);
        }

        ImGui::Separator();
        ImGui::InputFloat2("Figure Offset", &properties.figure.offset.x);

        if (ImGui::CollapsingHeader("Figure Rotate"))
        {

            ImGui::InputFloat2("Rotate Point", &properties.figure.rotate.point.x);
            ImGui::InputFloat("Rotate Angle", &properties.figure.rotate.angle);
        }

        ImGui::Checkbox("Render Figure", &properties.figure.render);
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

        ImGui::InputFloat2("R0", &properties.affine.R0.x);
        ImGui::InputFloat2("Rx", &properties.affine.Rx.x);
        ImGui::InputFloat2("Ry", &properties.affine.Ry.x);
    }
}

static void projective_properties()
{
    if (ImGui::CollapsingHeader("Projective"))
    {
        ImGui::InputFloat2("R0", &properties.projective.R0.x);
        ImGui::InputFloat2("Rx", &properties.projective.Rx.x);
        ImGui::InputFloat2("Ry", &properties.projective.Ry.x);
        ImGui::InputFloat2("W", &properties.projective.W.x);
        ImGui::InputFloat("W0", &properties.projective.W0);
        ImGui::Checkbox("Enable Projective", &properties.projective.enable);
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
