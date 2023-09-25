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

    struct Shader {
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

        void create(const std::string& vertex_file, const std::string& fragment_files)
        {
            std::string vertex_code   = read_file(vertex_file);
            std::string fragment_code = read_file(fragment_files);

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

        void use()
        {
            glUseProgram(shader);
        }

        void set(const char* name, const glm::vec2& vec)
        {
            GLint uniform = glGetUniformLocation(shader, name);
            glUniform2f(uniform, vec.r, vec.g);
        }

        void set(const char* name, const glm::vec4& vec)
        {
            GLint uniform = glGetUniformLocation(shader, name);
            glUniform4f(uniform, vec.r, vec.g, vec.b, vec.a);
        }

        void terminate()
        {
            glDeleteProgram(shader);
        }
    };

    struct OpenGL_State {
        Shader line_rendering_shader;
        Shader background_shader;


        void create_shader_programs()
        {
            line_rendering_shader.create("shaders/vertex.vert", "shaders/fragment.frag");
            background_shader.create("shaders/background.vert", "shaders/background.frag");
        }

        void init()
        {
            create_shader_programs();
        }

        void terminate()
        {
            line_rendering_shader.terminate();
            background_shader.terminate();
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

    void render_background()
    {
        opengl_state.background_shader.use();
        opengl_state.background_shader.set("ld_color", properties.rendering.ld_color);
        opengl_state.background_shader.set("lu_color", properties.rendering.lu_color);
        opengl_state.background_shader.set("rd_color", properties.rendering.rd_color);
        opengl_state.background_shader.set("ru_color", properties.rendering.ru_color);

        glDrawArrays(GL_TRIANGLES, 0, 6);
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
        opengl_state.line_rendering_shader.use();
        if (properties.rendering.use_props_colors)
        {
            opengl_state.line_rendering_shader.set("color1", properties.rendering.point1_color);
            opengl_state.line_rendering_shader.set("color2", properties.rendering.point2_color);
        }
        else
        {
            opengl_state.line_rendering_shader.set("color1", color1);
            opengl_state.line_rendering_shader.set("color2", color2);
        }

        opengl_state.line_rendering_shader.set("point1", normalize_coord(point1));
        opengl_state.line_rendering_shader.set("point2", normalize_coord(point2));

        glDrawArrays(GL_LINES, 0, 2);
    }
}// namespace Renderer
