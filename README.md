# Amőba / Five-in-a-Row (Gomoku)

**EN:** A Gomoku (5-in-a-row) game for two players or against the computer. Place stones on the board; the first to align 5 stones horizontally, vertically, or diagonally wins.

**HU:** Amőba játék (5-ös gomoku) két játékosnak vagy gép ellen. Rakj köveket a táblára; aki először rak ki 5 egyforma követ egymás mellé (vízszintesen, függőlegesen vagy átlósan), az nyer.

## Build / Fordítás

Open `GraphicsProject.cbp` in Code::Blocks and press **Build → Build** (F9).  
Requires MinGW/GCC with SDL2 and SDL2_ttf (included in `lib/` and `include/`).

## Controls / Vezérlés

| Action / Akció | Input |
|---|---|
| Place stone / Kő lerakása | Left click / Bal kattintás |
| New game / Új játék | **Új játék** button |
| Change board size / Méretváltoztatás | NumberSetter (15–30) → **Új játék** |
| Game mode / Játékmód | Selector: **2 jatekos** or **vs. Gep** |
| AI difficulty / Nehézség | NumberSetter (1–3): 1 = könnyű, 2 = közepes, 3 = nehéz |

## Changelog / Változások

- **v1.0** — Alap kétszemélyes játék / Base two-player game
- **v1.1** — Magyar felirat, pályakeret, hoshi pontok / Hungarian UI, board border, star points
- **v1.2** — Gépi játékos (heurisztikus AI) + módválasztó / AI opponent + mode selector
- **v1.3** — Nehézségállító (1–3), véletlenszerű AI / Difficulty setting, randomized AI
