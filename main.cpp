#include "window.hpp"
#include "button.hpp"
#include "label.hpp"
#include "boardwidget.hpp"
#include "gamemaster.hpp"
#include "menu.hpp"
#include <string>
using namespace std;

int main()
{
    Window w(700, 760);

    // Főmenü → játék → főmenü loop.
    // Beállítások (méret, mód, nehézség) kizárólag a főmenüből jönnek.
    for (GameSettings cfg = runMenu(); cfg.play; cfg = runMenu()) {
        GameMaster gm;
        gm.newGame(cfg.boardSize);

        // Topbar (y=5, h=50): [ÚjJáték:5,130][←Menü:140,110][Státusz:255,440]
        Label       status(   255,  5, 440,  50, "1. játékos lép");
        BoardWidget board(      0, 60, 700, 700, &gm);

        board.setOnMoveCallback([&](MoveResult result, int player) {
            if (result == MoveResult::WIN) {
                status.setText(to_string(player) + ". játékos nyert!");
            } else if (result == MoveResult::DRAW) {
                status.setText("Döntetlen! Új játékot?");
            } else {
                int next = (player == 1) ? 2 : 1;
                if (next == 2 && cfg.mode == 1)
                    status.setText("Gép lép...");
                else
                    status.setText(to_string(next) + ". játékos lép");
            }

            // vs. Gép: ember lépése után automatikus gépi lépés
            if (result == MoveResult::OK && player == 1
                && cfg.mode == 1
                && !gm.isGameOver()) {
                auto [aiCol, aiRow] = gm.computerMove(cfg.difficulty);
                MoveResult aiResult = gm.tryMove(aiCol, aiRow);
                if (aiResult == MoveResult::WIN)
                    status.setText("Gép nyert!");
                else if (aiResult == MoveResult::DRAW)
                    status.setText("Döntetlen! Új játékot?");
                else
                    status.setText("1. játékos lép");
            }
        });

        Button newGameBtn(  5,  5, 130,  50, "Új játék", [&]() {
            gm.newGame(cfg.boardSize);
            status.setText("1. játékos lép");
        });
        Button menuBtn(   140,  5, 110,  50, "<- Menü", [&]() {
            w.requestClose();
        });

        w.add(&newGameBtn);
        w.add(&menuBtn);
        w.add(&status);
        w.add(&board);

        w.run();
        w.clear(); // mutatók törlése a lokális változók megsemmisülése előtt
    }
    return 0;
}
