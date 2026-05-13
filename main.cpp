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

    //                     x    y   sx   sy  min  max  default
    NumberSetter sizeNS(  10,  10, 150,  40,  15,  30,  19);
    Label        status( 300,  10, 390,  40, "1. jatekos lep");
    BoardWidget  board(    0,  60, 700, 700, &gm);

    board.setOnMoveCallback([&](MoveResult result, int player) {
        if (result == MoveResult::WIN) {
            status.setText(to_string(player) + ". jatekos nyert!");
        } else if (result == MoveResult::DRAW) {
            status.setText("Dontetlen! Uj jatekot?");
        } else {
            int next = (player == 1) ? 2 : 1;
            status.setText(to_string(next) + ". jatekos lep");
        }
    });

    Button newGameBtn(170, 10, 120, 40, "Uj jatek", [&]() {
        gm.newGame(sizeNS.getInt());
        status.setText("1. jatekos lep");
    });

    w.add(&sizeNS);
    w.add(&newGameBtn);
    w.add(&status);
    w.add(&board);

    w.run();
    return 0;
}
