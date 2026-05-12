#ifndef NUMBERSETTER_HPP_INCLUDED
#define NUMBERSETTER_HPP_INCLUDED

#include "widgets.hpp"

class NumberSetter : public Widget {
    int _value, _min, _max;

    void clamp();
    bool overMinus(int mx, int my) const;
    bool overPlus(int mx, int my) const;
public:
    NumberSetter(int x, int y, int sx, int sy, int min, int max, int initial = 0);
    virtual void draw();
    virtual void handle(genv::event ev);
    virtual std::string getValue() const;
    int getInt() const;
};

#endif
