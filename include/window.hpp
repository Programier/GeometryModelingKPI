#pragma once


struct SDL_Window;
struct SDL_Renderer;

class Window
{
private:
    SDL_Window* _M_window;
    SDL_Renderer* _M_renderer;
    static Window* _M_instance;

    bool _M_is_first_frame;


    Window();


    void render_ui();
    void render_scene();

public:
    SDL_Window* window() const;
    SDL_Renderer* renderer() const;

    static Window* instance();
    void render();
    void wait_events() const;


    bool initialize();
    void terminate();

    ~Window();
};
