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

void Object::update()
{}

void Object::render()
{}

void Object::process_event(SDL_Event* event)
{}

Object::~Object()
{
    _M_objects.erase(this);
}
