#ifndef LABEL_HPP_INCLUDED
#define LABEL_HPP_INCLUDED

#include "widgets.hpp"
#include <string>

class Label : public Widget {
    std::string _text;
public:
    Label(int x, int y, int sx, int sy, std::string text = "");
    void setText(const std::string& t);
    std::string getValue() const override;
    void draw() override;
    void handle(genv::event ev) override {}
};

#endif
