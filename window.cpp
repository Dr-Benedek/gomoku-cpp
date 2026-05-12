#include "window.hpp"
#include "graphics.hpp"
#include <fstream>
using namespace genv;
using namespace std;

Window::Window(int width, int height)
    : _width(width), _height(height), _focus(-1)
{
    gout.open(width, height);
}

void Window::add(Widget* w)
{
    _widgets.push_back(w);
}

void Window::run()
{
    // alap háttér
    gout << move_to(0, 0) << color(30, 30, 30) << box(_width, _height);
    for (size_t i = 0; i < _widgets.size(); i++)
        if ((int)i != _focus) _widgets[i]->draw();
    if (_focus != -1) _widgets[_focus]->draw();
    gout << refresh;

    event ev;
    while (gin >> ev) {
        if (ev.type == ev_mouse) {
            if (ev.button == btn_left) {
                int new_focus = -1;
                // focused widget keeps priority if it still covers the click
                if (_focus != -1 && _widgets[_focus]->is_selected(ev.pos_x, ev.pos_y)) {
                    new_focus = _focus;
                } else {
                    for (size_t i = 0; i < _widgets.size(); i++) {
                        if (_widgets[i]->is_selected(ev.pos_x, ev.pos_y))
                            new_focus = (int)i;
                    }
                }
                if (new_focus != _focus) {
                    if (_focus != -1) _widgets[_focus]->setFocused(false);
                    _focus = new_focus;
                    if (_focus != -1) _widgets[_focus]->setFocused(true);
                }
            }
            // dispatch button events only to the single topmost widget at the cursor;
            // focused widget takes priority over widgets behind its expanded area
            if (ev.button != 0) {
                if (_focus != -1 && _widgets[_focus]->is_selected(ev.pos_x, ev.pos_y)) {
                    _widgets[_focus]->handle(ev);
                } else {
                    for (int i = (int)_widgets.size() - 1; i >= 0; i--) {
                        if (_widgets[i]->is_selected(ev.pos_x, ev.pos_y)) {
                            _widgets[i]->handle(ev);
                            break;
                        }
                    }
                }
            } else {
                for (Widget* w : _widgets) w->handle(ev);
            }
        //adatok mentése S gomb megnyomására
        } else if (ev.type == ev_key) {
            if (ev.keycode == 's') {
                ofstream f("values.txt");
                for (Widget* w : _widgets) {
                    string val = w->getValue();
                    if (!val.empty()) f << val << "\n";
                }
            }
            if (_focus != -1) _widgets[_focus]->handle(ev);
        }

        // ujrarajzolás / focus sorrendben / egymásra rajzolva
        gout << move_to(0, 0) << color(30, 30, 30) << box(_width, _height);
        for (size_t i = 0; i < _widgets.size(); i++)
            if ((int)i != _focus) _widgets[i]->draw();
        if (_focus != -1) _widgets[_focus]->draw();
        gout << refresh;
    }
}
