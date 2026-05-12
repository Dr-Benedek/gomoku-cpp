#include "button.hpp"
#include "graphics.hpp"
using namespace genv;
using namespace std;

static const int BTN_FONT = 14;

Button::Button(int x, int y, int sx, int sy,
               string label, function<void()> onClick)
    : Widget(x, y, sx, sy), _label(label), _onClick(onClick), _pressed(false)
{}

void Button::draw()
{
    gout.load_font("LiberationSans-Regular.ttf", BTN_FONT);
    int fonth = gout.cascent() + gout.cdescent();

    int br = _focused ? 80  : 100;
    int bg = _focused ? 140 : 100;
    int bb = _focused ? 255 : 100;

    int inner = _pressed ? 75 : 55;

    // border
    gout << move_to(_x, _y) << color(br, bg, bb) << box(_size_x, _size_y);
    // background
    gout << move_to(_x+1, _y+1) << color(inner, inner, inner) << box(_size_x-2, _size_y-2);

    // centered label
    int tw = gout.twidth(_label);
    int tx = _x + (_size_x - tw) / 2;
    int ty = _y + (_size_y - fonth) / 2;
    gout << move_to(tx, ty) << color(230, 230, 230) << text(_label);
}

void Button::handle(event ev)
{
    if (ev.type == ev_mouse) {
        if (ev.button == btn_left && is_selected(ev.pos_x, ev.pos_y)) {
            _pressed = true;
            if (_onClick) _onClick();
        } else {
            _pressed = false;
        }
    } else {
        _pressed = false;
    }
}
