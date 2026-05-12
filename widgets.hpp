#ifndef WIDGETS_HPP_INCLUDED
#define WIDGETS_HPP_INCLUDED

#include "graphics.hpp"
#include <string>

class Widget {
protected:
    int _x, _y, _size_x, _size_y;
    bool _focused;
public:
    Widget(int x, int y, int sx, int sy);
    virtual bool is_selected(int mouse_x, int mouse_y);
    void setFocused(bool f);
    virtual void draw() = 0;
    virtual void handle(genv::event ev) = 0;
    virtual std::string getValue() const;
    virtual ~Widget() {}
};

#endif
