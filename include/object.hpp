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
    virtual void process_event(SDL_Event* event);
    static void objects_process_event(SDL_Event* event);

    virtual ~Object();
};
