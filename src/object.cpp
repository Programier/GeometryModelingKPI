#include <object.hpp>

std::set<Object*> Object::_M_objects;

Object::Object()
{
    _M_objects.insert(this);
}

const std::set<Object*>& Object::objects()
{
    return _M_objects;
}

void Object::process_event(SDL_Event* event)
{}

void Object::objects_process_event(SDL_Event* event)
{
    for (Object* object : Object::objects())
    {
        object->process_event(event);
    }
}

Object::~Object()
{
    _M_objects.erase(this);
}
