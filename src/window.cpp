#include <opengl.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <application.hpp>
#include <cstring>
#include <definitions.hpp>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl2.h>
#include <iostream>
#include <object.hpp>
#include <renderer.hpp>
#include <vector>
#include <window.hpp>


static inline constexpr const char imgui_style[] = {
        0,    0,    -128, 63, -102, -103, 25,   63, 0,    0,    0,    65, 0,    0,    0,    65, 0,    0,    0,    0,
        0,    0,    -128, 63, 0,    0,    0,    66, 0,    0,    0,    66, 0,    0,    0,    0,  0,    0,    0,    63,
        0,    0,    0,    0,  0,    0,    0,    0,  0,    0,    -128, 63, 0,    0,    0,    0,  0,    0,    -128, 63,
        0,    0,    -128, 64, 0,    0,    64,   64, 0,    0,    64,   65, 0,    0,    0,    0,  0,    0,    0,    65,
        0,    0,    -128, 64, 0,    0,    -128, 64, 0,    0,    -128, 64, 0,    0,    -128, 64, 0,    0,    0,    64,
        0,    0,    0,    0,  0,    0,    0,    0,  0,    0,    -88,  65, 0,    0,    -64,  64, 0,    0,    96,   65,
        0,    0,    16,   65, 0,    0,    64,   65, 0,    0,    64,   65, 0,    0,    -128, 64, 0,    0,    -128, 64,
        0,    0,    0,    0,  0,    0,    0,    0,  0,    0,    -128, 63, 1,    0,    0,    0,  0,    0,    0,    63,
        0,    0,    0,    63, 0,    0,    0,    0,  0,    0,    0,    0,  0,    0,    64,   64, 0,    0,    0,    0,
        0,    0,    0,    63, 0,    0,    -96,  65, 0,    0,    64,   64, 0,    0,    -104, 65, 0,    0,    -104, 65,
        0,    0,    64,   64, 0,    0,    64,   64, 0,    0,    -128, 63, 1,    1,    1,    0,  0,    0,    -96,  63,
        -102, -103, -103, 62, 102,  102,  102,  63, 102,  102,  102,  63, 102,  102,  102,  63, 0,    0,    -128, 63,
        -102, -103, 25,   63, -102, -103, 25,   63, -102, -103, 25,   63, 0,    0,    -128, 63, 0,    0,    0,    0,
        0,    0,    0,    0,  0,    0,    0,    0,  -102, -103, 89,   63, 0,    0,    0,    0,  0,    0,    0,    0,
        0,    0,    0,    0,  0,    0,    0,    0,  -82,  71,   -31,  61, -82,  71,   -31,  61, 41,   92,   15,   62,
        31,   -123, 107,  63, 0,    0,    0,    63, 0,    0,    0,    63, 0,    0,    0,    63, 0,    0,    0,    63,
        0,    0,    0,    0,  0,    0,    0,    0,  0,    0,    0,    0,  0,    0,    0,    0,  -10,  40,   -36,  62,
        -10,  40,   -36,  62, -10,  40,   -36,  62, 20,   -82,  -57,  62, -41,  -93,  -16,  62, -41,  -93,  -16,  62,
        -41,  -93,  48,   63, -51,  -52,  -52,  62, 61,   10,   -41,  62, -123, -21,  -47,  62, 10,   -41,  35,   63,
        -41,  -93,  48,   63, 113,  61,   -118, 62, 113,  61,   -118, 62, 113,  61,   10,   63, -31,  122,  84,   63,
        10,   -41,  -93,  62, 10,   -41,  -93,  62, -82,  71,   33,   63, 82,   -72,  94,   63, -51,  -52,  -52,  62,
        -51,  -52,  -52,  62, -51,  -52,  76,   63, -51,  -52,  76,   62, -51,  -52,  -52,  62, -51,  -52,  -52,  62,
        -51,  -52,  12,   63, -51,  -52,  76,   63, -51,  -52,  76,   62, 0,    0,    -128, 62, -102, -103, -103, 62,
        -102, -103, 25,   63, -51,  -52,  -52,  62, -51,  -52,  -52,  62, -51,  -52,  76,   63, -102, -103, -103, 62,
        -51,  -52,  -52,  62, -51,  -52,  -52,  62, -51,  -52,  76,   63, -51,  -52,  -52,  62, -123, -21,  -47,  62,
        20,   -82,  -57,  62, -51,  -52,  76,   63, -102, -103, 25,   63, 102,  102,  102,  63, 102,  102,  102,  63,
        102,  102,  102,  63, 0,    0,    0,    63, 0,    0,    -128, 63, 0,    0,    -128, 63, 0,    0,    -128, 63,
        -102, -103, -103, 62, -123, -21,  -47,  62, 20,   -82,  -57,  62, -51,  -52,  76,   63, -102, -103, 25,   63,
        51,   51,   -77,  62, -51,  -52,  -52,  62, -10,  40,   28,   63, 82,   -72,  30,   63, -51,  -52,  -52,  62,
        -113, -62,  -11,  62, -113, -62,  53,   63, 113,  61,   74,   63, 31,   -123, -21,  62, 113,  61,   10,   63,
        -51,  -52,  76,   63, 0,    0,    -128, 63, -51,  -52,  -52,  62, -51,  -52,  -52,  62, 102,  102,  102,  63,
        102,  102,  -26,  62, 102,  102,  -26,  62, 102,  102,  -26,  62, 102,  102,  102,  63, -51,  -52,  76,   63,
        20,   -82,  7,    63, 20,   -82,  7,    63, 82,   -72,  94,   63, -51,  -52,  76,   63, 0,    0,    0,    63,
        0,    0,    0,    63, 0,    0,    0,    63, -102, -103, 25,   63, -102, -103, 25,   63, -102, -103, 25,   63,
        51,   51,   51,   63, 0,    0,    -128, 63, 51,   51,   51,   63, 51,   51,   51,   63, 102,  102,  102,  63,
        0,    0,    -128, 63, 0,    0,    -128, 63, 0,    0,    -128, 63, 0,    0,    -128, 63, -51,  -52,  -52,  61,
        20,   -82,  71,   63, -123, -21,  81,   63, 0,    0,    -128, 63, -102, -103, 25,   63, 20,   -82,  71,   63,
        -123, -21,  81,   63, 0,    0,    -128, 63, 102,  102,  102,  63, 49,   8,    -84,  62, 49,   8,    -84,  62,
        -96,  26,   47,   63, 76,   55,   73,   63, 102,  102,  -26,  62, 102,  102,  -26,  62, 102,  102,  102,  63,
        -51,  -52,  76,   63, 22,   -39,  -50,  62, 22,   -39,  -50,  62, 35,   -37,  57,   63, 80,   -115, 87,   63,
        -105, -1,   -112, 62, -105, -1,   -112, 62, -31,  -100, 17,   63, 42,   58,   82,   63, -95,  103,  -77,  62,
        -95,  103,  -77,  62, 66,   -49,  38,   63, -67,  82,   86,   63, 0,    0,    -128, 63, 0,    0,    -128, 63,
        0,    0,    -128, 63, 0,    0,    -128, 63, 102,  102,  102,  63, 51,   51,   51,   63, 0,    0,    0,    0,
        0,    0,    -128, 63, 102,  102,  102,  63, 51,   51,   51,   63, 0,    0,    0,    0,  0,    0,    -128, 63,
        0,    0,    -128, 63, -102, -103, 25,   63, 0,    0,    0,    0,  0,    0,    -128, 63, 113,  61,   -118, 62,
        113,  61,   -118, 62, 92,   -113, -62,  62, 0,    0,    -128, 63, 82,   -72,  -98,  62, 82,   -72,  -98,  62,
        102,  102,  -26,  62, 0,    0,    -128, 63, -72,  30,   -123, 62, -72,  30,   -123, 62, 41,   92,   -113, 62,
        0,    0,    -128, 63, 0,    0,    0,    0,  0,    0,    0,    0,  0,    0,    0,    0,  0,    0,    0,    0,
        0,    0,    -128, 63, 0,    0,    -128, 63, 0,    0,    -128, 63, 41,   92,   -113, 61, 0,    0,    0,    0,
        0,    0,    0,    0,  0,    0,    -128, 63, 51,   51,   -77,  62, 0,    0,    -128, 63, 0,    0,    -128, 63,
        0,    0,    0,    0,  102,  102,  102,  63, 102,  102,  -26,  62, 102,  102,  -26,  62, 102,  102,  102,  63,
        -51,  -52,  76,   63, 0,    0,    -128, 63, 0,    0,    -128, 63, 0,    0,    -128, 63, 51,   51,   51,   63,
        -51,  -52,  76,   63, -51,  -52,  76,   63, -51,  -52,  76,   63, -51,  -52,  76,   62, -51,  -52,  76,   62,
        -51,  -52,  76,   62, -51,  -52,  76,   62, 51,   51,   -77,  62, -102, -103, 25,   62, -102, -103, 25,   62,
        -51,  -52,  -52,  62, 0,    -92,  0,    0,  0,    4,    3,    0};

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


    Object::objects_process_event(event);
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
        skip_frames = 2;
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

    ImGuiStyle& style = ImGui::GetStyle();
    std::memcpy(&style, imgui_style, sizeof(style));

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
