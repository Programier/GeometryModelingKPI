#pragma once

struct SDL_Window;

class Window
{
private:
    SDL_Window* _M_window;
    void* _M_context;
    static Window* _M_instance;


    bool _M_is_first_frame;


    Window();


    void render_ui();
    void render_scene();

public:
    SDL_Window* window() const;

    static Window* instance();
    void render();
    void wait_events() const;


    bool initialize();
    void terminate();

    ~Window();
};
