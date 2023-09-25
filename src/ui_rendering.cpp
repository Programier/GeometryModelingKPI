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
        ImGui::SliderFloat4("Point1 color", &properties.rendering.point1_color.x, 0.0, 1.0);
        ImGui::SliderFloat4("Point2 color", &properties.rendering.point2_color.x, 0.0, 1.0);
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

    ImGui::End();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
