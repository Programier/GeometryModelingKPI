#include <application.hpp>
#include <object.hpp>
#include <properties.hpp>
#include <window.hpp>


Properties properties;

Application::Application()
{}

Application& Application::instance()
{
    static Application app;
    return app;
}

bool Application::init()
{
    _M_is_requested_exit = false;

    bool status = true;
    _M_window   = Window::instance();
    status      = _M_window->initialize();
    return status;
}

void Application::terminate()
{
    Window::instance()->terminate();
    delete Window::instance();
}

void Application::request_exit()
{
    _M_is_requested_exit = true;
}

bool Application::is_requested_exit() const
{
    return _M_is_requested_exit;
}


void Application::update()
{
    for (auto& object : Object::objects())
    {
        object->update();
    }
}

int Application::start()
{
    while (!is_requested_exit())
    {
        _M_window->wait_events();
        update();
        _M_window->render();
    }

    return 0;
}
