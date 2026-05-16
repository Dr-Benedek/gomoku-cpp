#include "window.hpp"
#include "numbersetter.hpp"
#include "button.hpp"
#include "selector.hpp"
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

    // Topbar elrendezese (y=10, h=40):
    // [Meret:10][NS:62][ModeSel:252][Status:362..690]
    // [UjJatek:152] kozvetlenul a jatekmezore szorva
    Label        meret(    10,  10,  52,  40, "Méret:");
    NumberSetter sizeNS(   62,  10,  90,  40,  15,  30,  19);
    Selector     modeSel(  252,  10, 110,  40, {"2 jatekos", "vs. Gep"}, 2);
    Label        status(   362,  10, 328,  40, "1. játékos lép");
    BoardWidget  board(      0,  60, 700, 700, &gm);

    board.setOnMoveCallback([&](MoveResult result, int player) {
        // Status frissitese az emberi lepes utan
        if (result == MoveResult::WIN) {
            status.setText(to_string(player) + ". játékos nyert!");
        } else if (result == MoveResult::DRAW) {
            status.setText("Döntetlen! Új játékot?");
        } else {
            int next = (player == 1) ? 2 : 1;
            if (next == 2 && modeSel.getValue() == "vs. Gep")
                status.setText("Gép lép...");
            else
                status.setText(to_string(next) + ". játékos lép");
        }

        // vs. Gep mod: ember lepese utan automatikus gepi lepes
        if (result == MoveResult::OK && player == 1
            && modeSel.getValue() == "vs. Gep"
            && !gm.isGameOver()) {
            auto [aiCol, aiRow] = gm.computerMove();
            MoveResult aiResult = gm.tryMove(aiCol, aiRow);
            if (aiResult == MoveResult::WIN)
                status.setText("Gép nyert!");
            else if (aiResult == MoveResult::DRAW)
                status.setText("Döntetlen! Új játékot?");
            else
                status.setText("1. játékos lép");
        }
    });

    Button newGameBtn(152, 10, 100, 40, "Új játék", [&]() {
        gm.newGame(sizeNS.getInt());
        status.setText("1. játékos lép");
    });

    w.add(&meret);
    w.add(&sizeNS);
    w.add(&newGameBtn);
    w.add(&modeSel);
    w.add(&status);
    w.add(&board);

    w.run();
    return 0;
}
