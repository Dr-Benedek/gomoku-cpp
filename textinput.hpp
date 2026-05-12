#ifndef TEXTINPUT_HPP_INCLUDED
#define TEXTINPUT_HPP_INCLUDED

#include "widgets.hpp"
#include <string>

class TextInput : public Widget {
    std::string _text;
public:
    TextInput(int x, int y, int sx, int sy);
    virtual void draw();
    virtual void handle(genv::event ev);
    virtual std::string getValue() const;
};

#endif
