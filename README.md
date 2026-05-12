# Amőba / Five-in-a-Row

**EN:** A two-player Gomoku (5-in-a-row) game. Select board size (15–30), press **Új játék** to start, then click on the board to place stones. First player to align 5 stones horizontally, vertically, or diagonally wins.

**HU:** Kétszemélyes amőba. Válaszd ki a pályaméretét (15–30), nyomj **Új játék** gombot, majd kattints a táblára a kövek lerakásához. Az nyer, aki először rak ki 5 egyforma követ egy sorba (vízszintesen, függőlegesen vagy átlósan).

## Build / Fordítás

Open `GraphicsProject.cbp` in Code::Blocks and press **Build → Build** (F9).  
Requires MinGW/GCC with SDL2 and SDL2_ttf (included in `lib/` and `include/`).

## Controls / Vezérlés

| Action / Akció | Input |
|---|---|
| Place stone / Kő lerakása | Left click / Bal kattintás |
| New game / Új játék | Click button / Gomb kattintás |
| Change board size / Méret változtatása | NumberSetter → Új játék |

## Changelog / Változások

- **v1.0** — Alap kétszemélyes játék / Base two-player game
