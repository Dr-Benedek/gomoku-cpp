#ifndef MENU_HPP_INCLUDED
#define MENU_HPP_INCLUDED

struct GameSettings {
    bool play;      // false = kilépés
    int  boardSize; // 15–30
    int  mode;      // 0 = "2 jatekos", 1 = "vs. Gep"
    int  difficulty;// 1–3
};

GameSettings runMenu();

#endif
