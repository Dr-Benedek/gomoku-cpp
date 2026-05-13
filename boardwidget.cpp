#include "boardwidget.hpp"
#include "graphics.hpp"
#include <cmath>
using namespace genv;

BoardWidget::BoardWidget(int x, int y, int sx, int sy, GameMaster* gm)
    : Widget(x, y, sx, sy), _gm(gm)
{}

void BoardWidget::setOnMoveCallback(std::function<void(MoveResult, int)> cb) {
    _onMove = cb;
}

int BoardWidget::cellSize() const {
    return _size_x / _gm->getBoardSize();
}

// Kitoltott kor rajzolasa vizszintes vonalakkal (hatekonyan O(r))
void BoardWidget::drawFilledCircle(int cx, int cy, int r,
                                   int red, int green, int blue) {
    gout << color(red, green, blue);
    for (int dy = -r; dy <= r; dy++) {
        int w = (int)std::sqrt((double)(r*r - dy*dy));
        gout << move_to(cx - w, cy + dy) << box(2*w + 1, 1);
    }
}

void BoardWidget::draw() {
    int size = _gm->getBoardSize();
    int cs   = cellSize();

    // Deszka hatter (fa szin)
    gout << move_to(_x, _y) << color(180, 140, 60) << box(_size_x, _size_y);

    // Racsvonalak
    gout << color(60, 40, 10);
    for (int i = 0; i <= size; i++) {
        gout << move_to(_x + i*cs, _y)        << line(0, _size_y);
        gout << move_to(_x,        _y + i*cs) << line(_size_x, 0);
    }

    // Nyero cellak elore lekerdezese
    auto winCells = _gm->getWinningCells();

    // Kovek rajzolasa
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            int cell = _gm->getCell(c, r);
            if (cell == 0) continue;

            int cx = _x + c*cs + cs/2;
            int cy = _y + r*cs + cs/2;
            int radius = cs/2 - 2;
            if (radius < 3) radius = 3;

            // Nyero ko: piros hatterkore
            bool isWinner = false;
            for (auto& wc : winCells)
                if (wc.first == c && wc.second == r) { isWinner = true; break; }

            if (isWinner)
                drawFilledCircle(cx, cy, radius + 3, 220, 50, 50);

            // 1. jatekos = fekete, 2. jatekos = feher
            if (cell == 1) drawFilledCircle(cx, cy, radius,  20,  20,  20);
            else           drawFilledCircle(cx, cy, radius, 230, 230, 230);
        }
    }
}

void BoardWidget::handle(genv::event ev) {
    if (ev.type   != ev_mouse)  return;
    if (ev.button != btn_left)  return;
    if (!is_selected(ev.pos_x, ev.pos_y)) return;
    if (_gm->isGameOver()) return;

    int cs  = cellSize();
    int col = (ev.pos_x - _x) / cs;
    int row = (ev.pos_y - _y) / cs;

    int player = _gm->getCurrentPlayer();
    MoveResult result = _gm->tryMove(col, row);

    if (result != MoveResult::INVALID && _onMove)
        _onMove(result, player);
}
