#include "selector.hpp"
#include "graphics.hpp"
#include <algorithm>
using namespace genv;
using namespace std;

static const int SEL_FONT = 14;

Selector::Selector(int x, int y, int sx, int sy,
                   vector<string> options, int maxVisible)
    : Widget(x, y, sx, sy),
      _options(options), _selected(0), _open(false), _scroll(0), _maxVisible(maxVisible)
{}

int Selector::dropdownHeight() const
{
    int visible = min((int)_options.size(), _maxVisible);
    return visible * _size_y;
}

bool Selector::is_selected(int mx, int my)
{
    if (Widget::is_selected(mx, my)) return true;
    if (_open) {
        int dh = dropdownHeight();
        if (mx >= _x && mx < _x + _size_x &&
            my >= _y + _size_y && my < _y + _size_y + dh)
            return true;
    }
    return false;
}

void Selector::draw()
{
    gout.load_font("LiberationSans-Regular.ttf", SEL_FONT);
    int fonth = gout.cascent() + gout.cdescent();

    int br = _focused ? 80  : 100;
    int bg = _focused ? 140 : 100;
    int bb = _focused ? 255 : 100;

    // keret
    gout << move_to(_x, _y) << color(br, bg, bb) << box(_size_x, _size_y);
    // háttér
    gout << move_to(_x+1, _y+1) << color(55, 55, 55) << box(_size_x-2, _size_y-2);

    // kiválasztott érték színe
    string sel_text = _options.empty() ? "" : _options[_selected];
    gout << move_to(_x + 6, _y + (_size_y - fonth)/2)
         << color(230, 230, 230) << text(sel_text);

    // háromszög jelző a legördülő ikon
    int tx = _x + _size_x - 16;
    int ty = _y + _size_y/2 - 3;
    for (int i = 0; i < 5; i++) {
        gout << move_to(tx + i, ty + i) << color(190, 190, 190) << box(9 - 2*i, 2);
    }

    // nyitott állapotban kirajzoljuk a legördülő listát
    if (_open) {
        int visible = min((int)_options.size(), _maxVisible);
        int dh = visible * _size_y;
        bool hasScroll = (int)_options.size() > _maxVisible;
        int sb_w = hasScroll ? 8 : 0;

        // háttér a legördülő listának
        gout << move_to(_x, _y + _size_y) << color(65, 65, 65) << box(_size_x, dh);

        for (int i = 0; i < visible; i++) {
            int idx = i + _scroll;
            if (idx >= (int)_options.size()) break;

            int iy = _y + _size_y + i * _size_y;

            // Kiemelés a jelenleg kiválasztott opcióra
            if (idx == _selected) {
                gout << move_to(_x, iy) << color(70, 105, 175) << box(_size_x - sb_w, _size_y);
            }

            // Elem szövege
            gout << move_to(_x + 6, iy + (_size_y - fonth)/2)
                 << color(230, 230, 230) << text(_options[idx]);

            // elválasztó vonal az elemek között (kivéve az utolsó alatt)
            if (i < visible - 1) {
                gout << move_to(_x, iy + _size_y - 1) << color(50, 50, 50) << box(_size_x - sb_w, 1);
            }
        }

        // Görgetősáv
        if (hasScroll) {
            int track_h = dh;
            int max_scroll = (int)_options.size() - _maxVisible;
            int thumb_h = max(12, track_h * _maxVisible / (int)_options.size());
            int thumb_y = _y + _size_y;
            if (max_scroll > 0)
                thumb_y += _scroll * (track_h - thumb_h) / max_scroll;

            gout << move_to(_x + _size_x - sb_w, _y + _size_y)
                 << color(45, 45, 45) << box(sb_w, track_h);
            gout << move_to(_x + _size_x - sb_w, thumb_y)
                 << color(130, 130, 130) << box(sb_w, thumb_h);
        }

        // keret a legördülő résznek
        gout << move_to(_x, _y + _size_y)           << color(br, bg, bb) << box(1, dh);
        gout << move_to(_x + _size_x - 1, _y + _size_y) << color(br, bg, bb) << box(1, dh);
        gout << move_to(_x, _y + _size_y + dh - 1)  << color(br, bg, bb) << box(_size_x, 1);
    }
}

void Selector::setFocused(bool f)
{
    Widget::setFocused(f);
    if (!f) _open = false;
}

void Selector::handle(event ev)
{
    if (ev.type == ev_mouse) {
        if (ev.button == btn_left) {
            if (Widget::is_selected(ev.pos_x, ev.pos_y)) {
                _open = !_open;
                if (_open) {
                    //kiválasztott elem láthatóvá tétele a legördülő listában (autüó legörgetés)
                    if (_selected < _scroll) _scroll = _selected;
                    if (_selected >= _scroll + _maxVisible)
                        _scroll = _selected - _maxVisible + 1;
                }
            } else if (_open) {
                int dh = dropdownHeight();
                if (ev.pos_x >= _x && ev.pos_x < _x + _size_x &&
                    ev.pos_y >= _y + _size_y && ev.pos_y < _y + _size_y + dh) {
                    // kattintás a legördülő listán, elem kiválasztás
                    int idx = (ev.pos_y - (_y + _size_y)) / _size_y + _scroll;
                    if (idx >= 0 && idx < (int)_options.size()) {
                        _selected = idx;
                    }
                }
                // kívül kattintás, bezárja a listát
                _open = false;
            }
        }
        // egérgörgő görgetés, ha nyitva van a lista
        if (ev.button == btn_wheelup && _open) {
            _scroll = max(0, _scroll - 1);
        }
        if (ev.button == btn_wheeldown && _open) {
            int max_scroll = (int)_options.size() - _maxVisible;
            _scroll = min(max_scroll, _scroll + 1);
        }
    }
    if (ev.type == ev_key && _open) {
        if (ev.keycode == key_up && _selected > 0) {
            _selected--;
            if (_selected < _scroll) _scroll = _selected;
        }
        if (ev.keycode == key_down && _selected < (int)_options.size() - 1) {
            _selected++;
            if (_selected >= _scroll + _maxVisible)
                _scroll = _selected - _maxVisible + 1;
        }
        if (ev.keycode == key_enter || ev.keycode == key_escape) {
            _open = false;
        }
    }
}

string Selector::getValue() const
{
    return _options.empty() ? "" : _options[_selected];
}

void Selector::addOption(const string& opt)
{
    _options.push_back(opt);
}

void Selector::removeSelected()
{
    if (_options.empty()) return;
    _options.erase(_options.begin() + _selected);
    if (_selected >= (int)_options.size() && _selected > 0)
        _selected--;
    _scroll = max(0, min(_scroll, (int)_options.size() - _maxVisible));
}
