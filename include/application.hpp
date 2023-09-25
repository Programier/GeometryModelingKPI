#pragma once
#include <definitions.hpp>

class Window;

class Application
{
private:
    Window* _M_window;
    bool _M_is_requested_exit;

    Application();

public:
    delete_copy_and_move(Application);

    static Application& instance();
    void request_exit();

    bool is_requested_exit() const;

    bool init();
    void terminate();
    int start();
};
