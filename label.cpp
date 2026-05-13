#include "label.hpp"
#include "graphics.hpp"
using namespace genv;
using namespace std;

static const int LBL_FONT = 14;

Label::Label(int x, int y, int sx, int sy, string text)
    : Widget(x, y, sx, sy), _text(text)
{}

void Label::setText(const string& t) {
    _text = t;
}

string Label::getValue() const {
    return _text;
}

void Label::draw() {
    gout.load_font("LiberationSans-Regular.ttf", LBL_FONT);
    int fonth = gout.cascent() + gout.cdescent();

    // Hatter (ablak hatterszinevel megegezo)
    gout << move_to(_x, _y) << color(30, 30, 30) << box(_size_x, _size_y);

    // Szoveg vertikalisan kozepre igazitva
    int ty = _y + (_size_y - fonth) / 2;
    gout << move_to(_x + 6, ty) << color(200, 200, 200) << text(_text);
}
