#include "textinput.hpp"
#include "graphics.hpp"
using namespace genv;
using namespace std;

static const int TI_FONT = 14;

TextInput::TextInput(int x, int y, int sx, int sy)
    : Widget(x, y, sx, sy)
{}

void TextInput::draw()
{
    gout.load_font("LiberationSans-Regular.ttf", TI_FONT);
    int fonth = gout.cascent() + gout.cdescent();
    int ty = _y + (_size_y - fonth) / 2;

    int br = _focused ? 80  : 100;
    int bg = _focused ? 140 : 100;
    int bb = _focused ? 255 : 100;

    // keret
    gout << move_to(_x, _y) << color(br, bg, bb) << box(_size_x, _size_y);
    // háttér
    gout << move_to(_x+1, _y+1) << color(40, 40, 40) << box(_size_x-2, _size_y-2);

    // csak azt a részt rajzoljuk ki a szövegből, ami elfér a mezőben
    int max_w = _size_x - 14;
    string display = _text;
    while (!display.empty() && gout.twidth(display) > max_w) {
        vector<string> chars = utf8_character_split(display);
        if (chars.size() <= 1) break;
        chars.erase(chars.begin());
        display = "";
        for (const string& c : chars) display += c;
    }

    gout << move_to(_x + 6, ty) << color(230, 230, 230) << text(display);

    // kurzor csak akkor, ha fókuszban van a mező
    if (_focused) {
        int cx = _x + 6 + gout.twidth(display);
        gout << move_to(cx, ty) << color(200, 200, 200) << box(2, fonth);
    }
}

void TextInput::handle(event ev)
{
    if (ev.type != ev_key) return;

    if (ev.keycode == key_backspace) {
        _text = utf8_remove_last(_text);
    } else if (ev.keycode >= 32 && ev.keycode < key_up && !ev.keyutf8.empty()) {
        _text += ev.keyutf8;
    }
}

string TextInput::getValue() const
{
    return _text;
}
