#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <application.hpp>
#include <definitions.hpp>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_sdlrenderer2.h>
#include <iostream>
#include <object.hpp>
#include <renderer.hpp>
#include <window.hpp>


SDL_Rect full_viewport;
SDL_Rect scene_viewport;


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

SDL_Renderer* Window::renderer() const
{
    return _M_renderer;
}

void Window::render_ui()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame(_M_window);
    ImGui::NewFrame();

    ImGui::Begin("Properties", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    if (_M_is_first_frame)
    {
        ImGui::SetWindowPos({0, 0});
        ImGui::SetWindowSize({PROPERTIES_WIDTH, WINDOW_HEIGHT});
    }

    ImGui::End();

    ImGui::Render();
}

void Window::render_scene()
{
    extern void render_scene();
    render_scene();
}

void Window::render()
{
    SDL_SetRenderDrawColor(_M_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_M_renderer);

    SDL_RenderSetViewport(_M_renderer, &full_viewport);
    render_ui();
    SDL_RenderSetViewport(_M_renderer, &scene_viewport);
    render_scene();


    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(_M_renderer);

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
                                 WINDOW_HEIGHT, SDL_WINDOW_SHOWN);


    if (!_M_window)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return false;
    }

    _M_renderer = SDL_CreateRenderer(_M_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!_M_renderer)
    {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return false;
    }

    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForSDLRenderer(_M_window, _M_renderer);
    ImGui_ImplSDLRenderer2_Init(_M_renderer);

    _M_is_first_frame = true;

    Renderer::init();

    full_viewport.x = 0;
    full_viewport.y = 0;
    full_viewport.w = WINDOW_WIDTH;
    full_viewport.h = WINDOW_HEIGHT;

    scene_viewport.x = PROPERTIES_WIDTH;
    scene_viewport.y = 0;
    scene_viewport.w = VIEWPORT_WIDTH;
    scene_viewport.h = WINDOW_HEIGHT;

    SDL_SetRenderDrawBlendMode(_M_renderer, SDL_BLENDMODE_BLEND);

    return true;
}

void Window::terminate()
{
    if (_M_window && _M_renderer)
    {
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    if (_M_renderer != nullptr)
    {
        SDL_DestroyRenderer(_M_renderer);
        _M_renderer = nullptr;
    }

    if (_M_window != nullptr)
    {
        SDL_DestroyWindow(_M_window);
        _M_window = nullptr;
    }


    SDL_Quit();
}

Window::~Window()
{
    terminate();
    _M_instance = nullptr;
}
