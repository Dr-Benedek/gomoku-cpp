#include "numbersetter.hpp"
#include "graphics.hpp"
#include <string>
using namespace genv;
using namespace std;

static const int NS_BTN_W  = 30;
static const int NS_FONT   = 14;

NumberSetter::NumberSetter(int x, int y, int sx, int sy, int min, int max, int initial)
    : Widget(x, y, sx, sy), _min(min), _max(max), _value(initial)
{
    clamp();
}

void NumberSetter::clamp()
{
    if (_value < _min) _value = _min;
    if (_value > _max) _value = _max;
}

bool NumberSetter::overMinus(int mx, int my) const
{
    return mx >= _x && mx < _x + NS_BTN_W && my >= _y && my < _y + _size_y;
}

bool NumberSetter::overPlus(int mx, int my) const
{
    return mx >= _x + _size_x - NS_BTN_W && mx < _x + _size_x
        && my >= _y && my < _y + _size_y;
}

void NumberSetter::draw()
{
    gout.load_font("LiberationSans-Regular.ttf", NS_FONT);
    int fonth = gout.cascent() + gout.cdescent();
    int ty = _y + (_size_y - fonth) / 2;

    // háttér, keret színe a fókusz állapotától függően
    int br = _focused ? 80  : 100;
    int bg = _focused ? 140 : 100;
    int bb = _focused ? 255 : 100;
    gout << move_to(_x, _y) << color(br, bg, bb) << box(_size_x, _size_y);

    // háttér
    gout << move_to(_x+1, _y+1) << color(55, 55, 55) << box(_size_x-2, _size_y-2);

    // [-] gomb
    gout << move_to(_x+1, _y+1) << color(75, 75, 75) << box(NS_BTN_W-2, _size_y-2);
    gout << move_to(_x + NS_BTN_W/2 - gout.twidth("-")/2, ty)
         << color(220, 220, 220) << text("-");

    // elválasztó bal
    gout << move_to(_x + NS_BTN_W, _y) << color(br, bg, bb) << box(1, _size_y);

    // [+] gomb
    gout << move_to(_x + _size_x - NS_BTN_W + 1, _y+1) << color(75, 75, 75) << box(NS_BTN_W-2, _size_y-2);
    gout << move_to(_x + _size_x - NS_BTN_W + NS_BTN_W/2 - gout.twidth("+")/2, ty)
         << color(220, 220, 220) << text("+");

    // elválasztó jobb
    gout << move_to(_x + _size_x - NS_BTN_W, _y) << color(br, bg, bb) << box(1, _size_y);

    // Érték
    string s = to_string(_value);
    int tw = gout.twidth(s);
    int cx = _x + NS_BTN_W + (_size_x - 2*NS_BTN_W - tw) / 2;
    gout << move_to(cx, ty) << color(230, 230, 230) << text(s);
}

void NumberSetter::handle(event ev)
{
    if (ev.type == ev_mouse && ev.button == btn_left) {
        if (overMinus(ev.pos_x, ev.pos_y)) { _value--; clamp(); }
        if (overPlus(ev.pos_x, ev.pos_y))  { _value++; clamp(); }
    }
    if (ev.type == ev_key) {
        if (ev.keycode == key_up)   { _value++;     clamp(); }
        if (ev.keycode == key_down) { _value--;     clamp(); }
        if (ev.keycode == key_pgup) { _value += 10; clamp(); }
        if (ev.keycode == key_pgdn) { _value -= 10; clamp(); }
    }
}

int NumberSetter::getInt() const
{
    return _value;
}

string NumberSetter::getValue() const
{
    return to_string(_value);
}
