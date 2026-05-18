#include "menu.hpp"
#include "graphics.hpp"
#include <cmath>
#include <string>
using namespace genv;
using namespace std;

static const int W = 700, H = 760;
static const char* MF = "LiberationSans-Regular.ttf";

// Teli kör rajzolás vízszintes scan-sorokkal
static void fillCircle(int cx, int cy, int r, int R, int G, int B) {
    gout << color(R, G, B);
    for (int dy = -r; dy <= r; dy++) {
        int w = (int)sqrt((double)(r*r - dy*dy));
        gout << move_to(cx - w, cy + dy) << box(2*w+1, 1);
    }
}

// Általános gomb
struct MBtn { int x, y, w, h; const char* label; bool hovered; };

static void drawMBtn(MBtn& b, int mx, int my, int fs = 20) {
    b.hovered = (mx >= b.x && mx < b.x+b.w && my >= b.y && my < b.y+b.h);
    int bg = b.hovered ?  75 :  50;
    int cg = b.hovered ? 140 : 100;
    int cb = b.hovered ? 255 : 150;
    gout << move_to(b.x,   b.y)   << color(80, cg, cb) << box(b.w,   b.h);
    gout << move_to(b.x+2, b.y+2) << color(bg, bg, bg) << box(b.w-4, b.h-4);
    gout.load_font(MF, fs);
    int tw = gout.twidth(b.label);
    int fh = gout.cascent() + gout.cdescent();
    gout << move_to(b.x+(b.w-tw)/2, b.y+(b.h-fh)/2)
         << color(225, 225, 225) << text(b.label);
}

// Beállítás sor: "Cimke:  [-] érték [+]"
// lbx = [-] gomb bal széle, rbx = [+] gomb bal széle
static bool settingMinus(int lbx, int rowy, int btnH, int mx, int my) {
    return mx >= lbx && mx < lbx+36 && my >= rowy+4 && my < rowy+4+btnH;
}
static bool settingPlus(int rbx, int rowy, int btnH, int mx, int my) {
    return mx >= rbx && mx < rbx+36 && my >= rowy+4 && my < rowy+4+btnH;
}

static void drawSetter(const string& lbl, const string& val,
                       int lbx, int rbx, int rowy, int rowH,
                       bool active, int mx, int my) {
    int btnH = rowH - 8;
    int btnY = rowy + 4;
    int lc   = active ? 200 : 90;

    // Cimke
    gout.load_font(MF, 17);
    int fh = gout.cascent() + gout.cdescent();
    gout << move_to(115, rowy + (rowH-fh)/2) << color(lc, lc, lc) << text(lbl);

    // [-] gomb
    bool lh = active && settingMinus(lbx, rowy, btnH, mx, my);
    gout << move_to(lbx,   btnY)   << color(70, 90, 170) << box(36, btnH);
    gout << move_to(lbx+2, btnY+2) << color(lh?88:55, lh?88:55, lh?88:55) << box(32, btnH-4);
    gout.load_font(MF, 18);
    int mw = gout.twidth("-"), mfh = gout.cascent()+gout.cdescent();
    gout << move_to(lbx+(36-mw)/2, btnY+(btnH-mfh)/2) << color(lc, lc, lc) << text("-");

    // Érték doboz
    int valX = lbx+38, valW = rbx-lbx-38-2;
    gout << move_to(valX, btnY) << color(35, 35, 52) << box(valW, btnH);
    gout.load_font(MF, 17);
    int vw = gout.twidth(val), vfh = gout.cascent()+gout.cdescent();
    int vc = active ? 230 : 90;
    gout << move_to(valX+(valW-vw)/2, btnY+(btnH-vfh)/2) << color(vc, active?205:90, active?80:50) << text(val);

    // [+] gomb
    bool rh = active && settingPlus(rbx, rowy, btnH, mx, my);
    gout << move_to(rbx,   btnY)   << color(70, 90, 170) << box(36, btnH);
    gout << move_to(rbx+2, btnY+2) << color(rh?88:55, rh?88:55, rh?88:55) << box(32, btnH-4);
    gout.load_font(MF, 18);
    int pw = gout.twidth("+");
    gout << move_to(rbx+(36-pw)/2, btnY+(btnH-mfh)/2) << color(lc, lc, lc) << text("+");
}

// Mód toggle gomb (kattintásra vált)
static void drawModeBtn(int mode, int mx, int my, int rowy, int rowH) {
    gout.load_font(MF, 17);
    int fh = gout.cascent() + gout.cdescent();
    gout << move_to(115, rowy+(rowH-fh)/2) << color(200, 200, 200) << text("Mód:");

    int bx = 265, by = rowy+4, bw = 170, bh = rowH-8;
    bool hov = (mx >= bx && mx < bx+bw && my >= by && my < by+bh);
    gout << move_to(bx,   by)   << color(80, hov?140:100, hov?255:150) << box(bw,   bh);
    gout << move_to(bx+2, by+2) << color(hov?70:45, hov?70:45, hov?70:45) << box(bw-4, bh-4);

    string mstr = (mode == 0) ? "2 jatekos" : "vs. Gep";
    int tw = gout.twidth(mstr);
    gout << move_to(bx+(bw-tw)/2, by+(bh-fh)/2) << color(225, 225, 225) << text(mstr);
}

// Háttér rácssal és dekorációs kövekkel
static void drawBg() {
    gout << move_to(0, 0) << color(18, 18, 28) << box(W, H);
    gout << color(27, 27, 44);
    for (int x = 0; x <= W; x += 35) gout << move_to(x, 0) << line(0, H);
    for (int y = 0; y <= H; y += 35) gout << move_to(0, y) << line(W, 0);
    fillCircle( 60, 130, 17,  38,  38,  38);
    fillCircle(640, 200, 14,  38,  38,  38);
    fillCircle( 45, 620, 16,  38,  38,  38);
    fillCircle(655, 590, 13,  38,  38,  38);
    fillCircle(680, 420, 11,  38,  38,  38);
    fillCircle( 90, 165, 15, 212, 207, 192);
    fillCircle(615, 235, 12, 212, 207, 192);
    fillCircle( 70, 655, 14, 212, 207, 192);
    fillCircle(630, 555, 15, 212, 207, 192);
}

GameSettings runMenu() {
    gout.load_font(MF, 20);

    GameSettings cfg = {true, 19, 0, 2};

    // Főmenü gombok
    MBtn playBtn = {W/2-120, 468, 240, 55, "Játék!",      false};
    MBtn infoBtn = {W/2-120, 533, 240, 48, "Súgó / Info", false};
    MBtn quitBtn = {W/2-120, 591, 240, 48, "Kilépés",     false};
    // Info képernyő vissza gomb
    MBtn backBtn = {W/2-85,  595, 170, 46, "Vissza",      false};

    // Beállítás sorok y-koordinátái és a [-]/[+] gombok x-pozíciói
    static const int LBX = 265, RBX = 363; // left btn x, right btn x (= LBX+36+val_w+2)
    static const int RY_MOD  = 292;        // Mód sor
    static const int RY_SZ   = 342;        // Pályaméret sor
    static const int RY_DIFF = 392;        // Nehézség sor
    static const int RH = 44;              // sor magassága

    bool infoOpen = false;
    int mx = 0, my = 0;

    event ev;
    while (gin >> ev) {
        if (ev.type == ev_key && ev.keycode == key_escape) {
            cfg.play = false; return cfg;
        }
        if (ev.type == ev_mouse) { mx = ev.pos_x; my = ev.pos_y; }

        if (infoOpen) {
            // ── Info képernyő ──────────────────────────────────────────
            drawBg();
            int px = 70, py = 80, pw = 560, ph = 575;
            gout << move_to(px, py) << color(12, 12, 22) << box(pw, ph);
            gout << color(85, 85, 130);
            gout << move_to(px,    py)    << line(pw,  0);
            gout << move_to(px,    py)    << line(0,  ph);
            gout << move_to(px+pw, py)    << line(0,  ph);
            gout << move_to(px,    py+ph) << line(pw,  0);

            gout.load_font(MF, 24);
            string ptitle = "Játékszabályok / Rules";
            gout << move_to(W/2 - gout.twidth(ptitle)/2, py+30)
                 << color(255, 215, 55) << text(ptitle);

            gout.load_font(MF, 16);
            int fh = gout.cascent() + gout.cdescent() + 7;
            int ty = py + 82;
            auto ln = [&](const string& s, int r=190, int g=190, int b=190) {
                gout << move_to(px+28, ty) << color(r, g, b) << text(s);
                ty += fh;
            };

            ln("Cél:", 255, 215, 55);
            ln("  Tedd le először 5 követ egymás mellé –");
            ln("  vízszintesen, függőlegesen vagy átlósan.");
            ty += 6;
            ln("Főmenü beállítások:", 255, 215, 55);
            ln("  Mód        →  2 jatekos (helyi) / vs. Gep (AI)");
            ln("  Pályaméret →  15–30, [-] [+] gombokkal");
            ln("  Nehézség   →  1–3, csak vs. Gép módban aktív");
            ty += 6;
            ln("Nehézség szintek:", 255, 215, 55);
            ln("  1 = Könnyű   (nagy véletlenszerűség)");
            ln("  2 = Közepes  (alapértelmezett)");
            ln("  3 = Nehéz    (legjobb lépés)");
            ty += 6;
            ln("Játék közben:", 255, 215, 55);
            ln("  Bal katt      →  kő lerakása");
            ln("  Új játék      →  újrakezd (azonos beáll.)");
            ln("  <- Menü       →  vissza a főmenübe");

            drawMBtn(backBtn, mx, my, 17);

            if (ev.type == ev_mouse && ev.button == btn_left && backBtn.hovered)
                infoOpen = false;

        } else {
            // ── Főmenü ────────────────────────────────────────────────
            drawBg();

            // Cím
            gout.load_font(MF, 54);
            string t1 = "AMŐBA";
            gout << move_to(W/2 - gout.twidth(t1)/2, 95)
                 << color(255, 215, 55) << text(t1);
            gout.load_font(MF, 20);
            string t2 = "Five in a row  ·  Gomoku";
            gout << move_to(W/2 - gout.twidth(t2)/2, 172)
                 << color(155, 155, 155) << text(t2);

            // 5 kő dekoráció
            for (int i = 0; i < 5; i++)
                fillCircle(W/2 - 100 + i*50, 228, 17, 42, 42, 42);
            fillCircle(W/2, 228, 17, 210, 205, 190);

            // Beállítások szekció fejléc
            gout.load_font(MF, 13);
            string sep = "─── Beállítások ───";
            gout << move_to(W/2 - gout.twidth(sep)/2, 270)
                 << color(85, 85, 118) << text(sep);

            // Beállítás sorok
            drawModeBtn(cfg.mode, mx, my, RY_MOD, RH);
            drawSetter("Pályaméret:", to_string(cfg.boardSize),
                       LBX, RBX, RY_SZ, RH, true, mx, my);
            drawSetter("Nehézség:",   to_string(cfg.difficulty),
                       LBX, RBX, RY_DIFF, RH, cfg.mode == 1, mx, my);

            // Elválasztó vonal a gombok előtt
            gout << move_to(70, 448) << color(38, 38, 62) << box(560, 1);

            drawMBtn(playBtn, mx, my, 22);
            drawMBtn(infoBtn, mx, my, 17);
            drawMBtn(quitBtn, mx, my, 17);

            if (ev.type == ev_mouse && ev.button == btn_left) {
                // Mód toggle
                int by = RY_MOD+4, bh = RH-8;
                if (mx >= 265 && mx < 435 && my >= by && my < by+bh)
                    cfg.mode = 1 - cfg.mode;

                // Pályaméret
                if (settingMinus(LBX, RY_SZ, RH-8, mx, my))
                    cfg.boardSize = max(15, cfg.boardSize - 1);
                if (settingPlus(RBX, RY_SZ, RH-8, mx, my))
                    cfg.boardSize = min(30, cfg.boardSize + 1);

                // Nehézség (csak vs. Gép módban)
                if (cfg.mode == 1) {
                    if (settingMinus(LBX, RY_DIFF, RH-8, mx, my))
                        cfg.difficulty = max(1, cfg.difficulty - 1);
                    if (settingPlus(RBX, RY_DIFF, RH-8, mx, my))
                        cfg.difficulty = min(3, cfg.difficulty + 1);
                }

                if (playBtn.hovered)              return cfg;
                if (infoBtn.hovered)              infoOpen = true;
                if (quitBtn.hovered) { cfg.play = false; return cfg; }
            }
        }

        gout << refresh;
    }
    cfg.play = false;
    return cfg;
}
