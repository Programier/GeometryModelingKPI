#pragma once
#include <set>


union SDL_Event;


class Object
{
private:
    static std::set<Object*> _M_objects;

public:
    Object();

    static const std::set<Object*>& objects();

    virtual void update();
    virtual void render();
    virtual void process_event(SDL_Event* event);

    virtual ~Object();
};
