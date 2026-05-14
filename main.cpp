#include "window.hpp"
#include "numbersetter.hpp"
#include "button.hpp"
#include "label.hpp"
#include "boardwidget.hpp"
#include "gamemaster.hpp"
#include <string>
using namespace std;

int main()
{
    Window w(700, 760);

    GameMaster gm;
    gm.newGame(19);

    //                      x    y   sx   sy
    Label        meret(    10,  10,  55,  40, "Meret:");
    //                      x    y   sx   sy  min  max  default
    NumberSetter sizeNS(   65,  10,  95,  40,  15,  30,  19);
    //                      x    y   sx   sy
    Label        status(  281,  10, 409,  40, "1. játékos lép");
    BoardWidget  board(     0,  60, 700, 700, &gm);

    board.setOnMoveCallback([&](MoveResult result, int player) {
        if (result == MoveResult::WIN) {
            status.setText(to_string(player) + ". játékos nyert!");
        } else if (result == MoveResult::DRAW) {
            status.setText("Döntetlen! Új játékot?");
        } else {
            int next = (player == 1) ? 2 : 1;
            status.setText(to_string(next) + ". játékos lép");
        }
    });

    Button newGameBtn(168, 10, 105, 40, "Új játék", [&]() {
        gm.newGame(sizeNS.getInt());
        status.setText("1. játékos lép");
    });

    w.add(&meret);
    w.add(&sizeNS);
    w.add(&newGameBtn);
    w.add(&status);
    w.add(&board);

    w.run();
    return 0;
}
