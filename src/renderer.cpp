#include <definitions.hpp>
#include <opengl.hpp>

#include <SDL2/SDL.h>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <properties.hpp>
#include <renderer.hpp>
#include <string>
#include <window.hpp>


namespace Renderer
{
    static SDL_Window* window;
    SDL_GLContext context;

    static std::string read_file(const std::string& filename)
    {
        std::ifstream file(filename);
        if (file.is_open())
        {
            std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            file.close();
            return text;
        }
        return "";
    }

    struct OpenGL_State {
        GLuint shader;

        GLuint compile_shader(const std::string& code, GLenum type)
        {
            printf("Compiling %s shader\n", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"));

            GLuint shader_id   = glCreateShader(type);
            const GLchar* text = reinterpret_cast<const GLchar*>(code.c_str());
            glShaderSource(shader_id, 1, &text, nullptr);
            glCompileShader(shader_id);

            GLint success;
            glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                GLchar info_log[512];
                glGetShaderInfoLog(shader_id, sizeof(info_log), nullptr, info_log);
                std::cerr << info_log << std::endl;
                glDeleteShader(shader_id);
                std::exit(-1);
            }

            return shader_id;
        }

        void create_shader_program()
        {
            std::string vertex_code   = read_file("shaders/vertex.vert");
            std::string fragment_code = read_file("shaders/fragment.frag");

            GLuint vertex   = compile_shader(vertex_code, GL_VERTEX_SHADER);
            GLuint fragment = compile_shader(fragment_code, GL_FRAGMENT_SHADER);

            // Створення програми шейдерів
            shader = glCreateProgram();
            glAttachShader(shader, vertex);
            glAttachShader(shader, fragment);
            glLinkProgram(shader);

            // Перевірка на помилки лінкування програми шейдерів
            GLint success;
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                GLchar info_log[512];
                glGetProgramInfoLog(shader, sizeof(info_log), nullptr, info_log);
                std::cerr << info_log << std::endl;
            }

            // Видалення непотрібних об'єктів
            glDeleteShader(vertex);
            glDeleteShader(fragment);

            glUseProgram(shader);
        }


        void set_position(const glm::vec2& pos, unsigned int index)
        {
            static std::string name = "point0";
            name[5]                 = '0' + index;

            GLint uniform = glGetUniformLocation(shader, name.c_str());
            glUniform2f(uniform, pos.x, pos.y);
        }

        void set_color(const glm::vec4& color, unsigned int index)
        {
            static std::string name = "color0";
            name[5]                 = '0' + index;

            GLint uniform = glGetUniformLocation(shader, name.c_str());
            glUniform4f(uniform, color.r, color.g, color.b, color.a);
        }

        void init()
        {
            create_shader_program();
        }

        void terminate()
        {
            glDeleteProgram(shader);
        }
    } opengl_state;

    void init()
    {
        window = Window::instance()->window();
        opengl_state.init();

        glDisable(GL_DEPTH_TEST);
    }

    void terminate()
    {
        opengl_state.terminate();
    }

    /////////////////////// LINE RENDERING ///////////////////////

    static glm::vec2 normalize_coord(glm::ivec2 point)
    {
        static constexpr float width  = static_cast<float>(VIEWPORT_WIDTH);
        static constexpr float height = static_cast<float>(VIEWPORT_HEIGHT);

        float X = (2.0f * static_cast<float>(point.x) / width) - 1.0f;
        float Y = (2.0f * static_cast<float>(point.y) / height) - 1.0f;
        return {X, Y};
    }

    void draw_line(glm::ivec2 point1, glm::ivec2 point2, const glm::vec4& color1, const glm::vec4& color2)
    {
        glUseProgram(opengl_state.shader);
        if (properties.rendering.use_props_colors)
        {
            opengl_state.set_color(properties.rendering.point1_color, 1);
            opengl_state.set_color(properties.rendering.point2_color, 2);
        }
        else
        {
            opengl_state.set_color(color1, 1);
            opengl_state.set_color(color2, 2);
        }
        opengl_state.set_position(normalize_coord(point1), 1);
        opengl_state.set_position(normalize_coord(point2), 2);
        glDrawArrays(GL_LINES, 0, 2);
    }
}// namespace Renderer
