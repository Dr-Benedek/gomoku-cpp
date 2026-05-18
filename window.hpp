#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLUDED

#include "graphics.hpp"
#include "widgets.hpp"
#include <vector>

class Window {
    std::vector<Widget*> _widgets;
    int _width, _height;
    int _focus;
    bool _shouldClose;
public:
    Window(int width, int height);
    void add(Widget* w);
    void clear();
    void requestClose();
    void run();
};

#endif
