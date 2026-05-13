#ifndef BOARDWIDGET_HPP_INCLUDED
#define BOARDWIDGET_HPP_INCLUDED

#include "widgets.hpp"
#include "gamemaster.hpp"
#include <functional>

class BoardWidget : public Widget {
public:
    BoardWidget(int x, int y, int sx, int sy, GameMaster* gm);
    void draw() override;
    void handle(genv::event ev) override;
    void setOnMoveCallback(std::function<void(MoveResult, int)> cb);

private:
    GameMaster* _gm;
    std::function<void(MoveResult, int)> _onMove;

    int cellSize() const;
    void drawFilledCircle(int cx, int cy, int r, int red, int green, int blue);
};

#endif
