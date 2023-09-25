
#include <opengl.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <application.hpp>
#include <definitions.hpp>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl2.h>
#include <iostream>
#include <object.hpp>
#include <renderer.hpp>
#include <window.hpp>


Window* Window::_M_instance = nullptr;


Window::Window() : _M_window(nullptr)
{}


Window* Window::instance()
{
    if (!_M_instance)
    {
        _M_instance = new Window();
    }

    return _M_instance;
}

SDL_Window* Window::window() const
{
    return _M_window;
}


void Window::render_scene()
{
    glViewport(PROPERTIES_WIDTH, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    Renderer::render_background();
    extern void render_scene();
    render_scene();
}

void Window::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    render_ui();
    render_scene();

    SDL_GL_SwapWindow(_M_window);
    _M_is_first_frame = false;
}


static void process_event(SDL_Event* event)
{
    if (event->type == SDL_QUIT)
    {
        Application::instance().request_exit();
    }

    ImGui_ImplSDL2_ProcessEvent(event);
}

void Window::wait_events() const
{
    SDL_Event event;
    static int skip_frames = 2;
    if (skip_frames > 0)
    {
        --skip_frames;
    }
    else
    {
        SDL_WaitEvent(&event);
        process_event(&event);
    }


    while (SDL_PollEvent(&event))
    {
        process_event(&event);
    }
}

bool Window::initialize()
{
    if (_M_window)
        return false;

    SDL_Init(SDL_INIT_EVERYTHING);

#if PLATFORM_LINUX
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
#endif


    _M_window = SDL_CreateWindow("Geometry modeling", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                                 WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);


    if (!_M_window)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);


    _M_context = SDL_GL_CreateContext(_M_window);
    SDL_GL_MakeCurrent(_M_window, _M_context);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    ImGui::CreateContext();

    ImGui_ImplSDL2_InitForOpenGL(_M_window, _M_context);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    Renderer::init();

    _M_is_first_frame = true;

    return true;
}

void Window::terminate()
{
    Renderer::terminate();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();


    SDL_DestroyWindow(_M_window);
    _M_window = nullptr;


    SDL_Quit();
}

Window::~Window()
{
    terminate();
    _M_instance = nullptr;
}
