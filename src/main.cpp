#include <application.hpp>
#include <iostream>


int main()
{
    Application& application = Application::instance();

    if (!application.init())
    {
        std::clog << "Failed to create window!" << std::endl;
        return -1;
    }

    int result = application.start();
    application.terminate();
    return result;
}
