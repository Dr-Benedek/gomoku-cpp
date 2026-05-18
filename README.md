# Amőba / Five-in-a-Row (Gomoku)

**EN:** A Gomoku (5-in-a-row) game. Play against a friend or the computer. Configure all settings in the main menu, then click **Játék!** to start.

**HU:** Amőba játék (gomoku). Játssz barát ellen vagy gép ellen. Állítsd be a beállításokat a főmenüben, majd kattints a **Játék!** gombra.

## Build / Fordítás

Open `GraphicsProject.cbp` in Code::Blocks and press **Build → Build** (F9).  
Requires MinGW/GCC with SDL2 and SDL2_ttf (included in `lib/` and `include/`).

## Main Menu / Főmenü

Configure before starting — all settings live here.

| Setting / Beállítás | Options / Lehetőségek |
|---|---|
| Mód | `2 jatekos` (local 2-player) / `vs. Gep` (AI) |
| Pályaméret | 15–30 via `[-]` `[+]` |
| Nehézség | 1–3, only active in `vs. Gep` mode |

**Súgó / Info** — game rules and controls.

## In-Game Controls / Játék közbeni vezérlés

| Action / Akció | Control |
|---|---|
| Place stone / Kő lerakása | Left click / Bal kattintás |
| Restart with same settings / Újrakezd | **Új játék** button |
| Back to menu / Főmenübe | **← Menü** button |

To change board size, mode or difficulty, go back to the main menu via **← Menü**.

## Difficulty / Nehézség (vs. Gép)

| Level | Behaviour |
|---|---|
| 1 – Könnyű | Large random noise — plays weaker |
| 2 – Közepes | Small noise — balanced (default) |
| 3 – Nehéz | No noise, picks among best moves randomly |

## Changelog / Változások

- **v1.0** — Alap kétszemélyes játék / Base two-player game
- **v1.1** — Magyar felirat, pályakeret, hoshi pontok / Hungarian UI, board border, star points
- **v1.2** — Gépi játékos (heurisztikus AI) + módválasztó / AI opponent + mode selector
- **v1.3** — Nehézségállító (1–3), véletlenszerű AI / Difficulty setting, randomized AI
- **v1.4** — Főmenü beállításokkal, ← Menü gomb / Main menu with settings, back-to-menu button
- **v1.5** — Egyszerűsített topbar, beállítások csak a főmenüben / Simplified topbar, settings in menu only
