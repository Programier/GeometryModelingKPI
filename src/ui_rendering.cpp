#include <definitions.hpp>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl2.h>
#include <opengl.hpp>
#include <properties.hpp>
#include <window.hpp>


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
            ImGui::SliderFloat3("Vertical Color1", &properties.grid.v_color1.x, 0.0, 1.0);
            ImGui::SliderFloat3("Vertical Color2", &properties.grid.v_color2.x, 0.0, 1.0);
        }

        if (ImGui::CollapsingHeader("Horizontal Lines"))
        {
            ImGui::SliderFloat3("Horizontal Color1", &properties.grid.h_color1.x, 0.0, 1.0);
            ImGui::SliderFloat3("Horizontal Color2", &properties.grid.h_color2.x, 0.0, 1.0);
        }

        ImGui::Checkbox("Disable Grid", &properties.grid.disable_grid);
        ImGui::Checkbox("Disable Axis", &properties.grid.disable_axis);
    }
}

static void object_properties()
{
    if (ImGui::CollapsingHeader("Object"))
    {
        for (Property& prop : properties.figure.props)
        {
            if (ImGui::SliderFloat(prop.name, &prop.value, 0.0f, 50.f))
            {
                // TODO
            }
        }
    }
}

static void reset_properties()
{
    new (&properties) Properties();
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
    grid_properties();
    object_properties();

    if (ImGui::Button("Reset"))
    {
        reset_properties();
    }

    ImGui::End();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
