#ifndef GAMEMASTER_HPP_INCLUDED
#define GAMEMASTER_HPP_INCLUDED

#include <vector>
#include <utility>

enum class MoveResult { OK, INVALID, WIN, DRAW };

class GameMaster {
public:
    GameMaster();
    void newGame(int size);
    MoveResult tryMove(int col, int row);
    int getCell(int col, int row) const;
    int getCurrentPlayer() const;
    int getBoardSize() const;
    bool isGameOver() const;
    bool getIsDraw() const;
    std::vector<std::pair<int,int>> getWinningCells() const;

    // Gepi lepesvalasztas (heurisztikus pontozas)
    std::pair<int,int> computerMove();

private:
    std::vector<std::vector<int>> _board;
    int _currentPlayer;
    int _boardSize;
    bool _gameOver;
    bool _isDraw;
    int _moveCount;
    std::vector<std::pair<int,int>> _winningCells;

    bool checkWin(int col, int row);
    int  countLine(int col, int row, int dx, int dy, int player) const;
};

#endif
