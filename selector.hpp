#ifndef SELECTOR_HPP_INCLUDED
#define SELECTOR_HPP_INCLUDED

#include "widgets.hpp"
#include <vector>
#include <string>

class Selector : public Widget {
    std::vector<std::string> _options;
    int _selected;
    bool _open;
    int _scroll;
    int _maxVisible;

    int dropdownHeight() const;
public:
    Selector(int x, int y, int sx, int sy,
             std::vector<std::string> options, int maxVisible);
    virtual bool is_selected(int mx, int my);
    virtual void setFocused(bool f);
    virtual void draw();
    virtual void handle(genv::event ev);
    virtual std::string getValue() const;
    void addOption(const std::string& opt);
    void removeSelected();
};

#endif
