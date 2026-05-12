#ifndef BUTTON_HPP_INCLUDED
#define BUTTON_HPP_INCLUDED

#include "widgets.hpp"
#include <string>
#include <functional>

class Button : public Widget {
    std::string _label;
    std::function<void()> _onClick;
    bool _pressed;
public:
    Button(int x, int y, int sx, int sy,
           std::string label, std::function<void()> onClick);
    virtual void draw();
    virtual void handle(genv::event ev);
};

#endif
