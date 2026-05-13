#include "gamemaster.hpp"
#include <cmath>

GameMaster::GameMaster()
    : _currentPlayer(1), _boardSize(19), _gameOver(false),
      _isDraw(false), _moveCount(0)
{
    newGame(19);
}

void GameMaster::newGame(int size) {
    _boardSize     = size;
    _currentPlayer = 1;
    _gameOver      = false;
    _isDraw        = false;
    _moveCount     = 0;
    _winningCells.clear();
    _board.assign(size, std::vector<int>(size, 0));
}

int GameMaster::getCell(int col, int row) const {
    if (col < 0 || col >= _boardSize || row < 0 || row >= _boardSize) return 0;
    return _board[row][col];
}

int  GameMaster::getCurrentPlayer() const { return _currentPlayer; }
int  GameMaster::getBoardSize()     const { return _boardSize; }
bool GameMaster::isGameOver()       const { return _gameOver; }
bool GameMaster::getIsDraw()        const { return _isDraw; }

std::vector<std::pair<int,int>> GameMaster::getWinningCells() const {
    return _winningCells;
}

MoveResult GameMaster::tryMove(int col, int row) {
    if (_gameOver) return MoveResult::INVALID;
    if (col < 0 || col >= _boardSize || row < 0 || row >= _boardSize)
        return MoveResult::INVALID;
    if (_board[row][col] != 0) return MoveResult::INVALID;

    _board[row][col] = _currentPlayer;
    _moveCount++;

    if (checkWin(col, row)) {
        _gameOver = true;
        return MoveResult::WIN;
    }

    if (_moveCount == _boardSize * _boardSize) {
        _gameOver = true;
        _isDraw   = true;
        return MoveResult::DRAW;
    }

    _currentPlayer = (_currentPlayer == 1) ? 2 : 1;
    return MoveResult::OK;
}

bool GameMaster::checkWin(int col, int row) {
    int player = _board[row][col];
    // 4 irany: jobbra, le, jobb-le atlo, jobb-fel atlo
    int dirs[4][2] = {{1,0},{0,1},{1,1},{1,-1}};

    for (auto& d : dirs) {
        int dx = d[0], dy = d[1];
        std::vector<std::pair<int,int>> cells;
        cells.push_back({col, row});

        // Pozitiv irany
        for (int i = 1; i <= 4; i++) {
            int nc = col + dx*i, nr = row + dy*i;
            if (nc < 0 || nc >= _boardSize || nr < 0 || nr >= _boardSize) break;
            if (_board[nr][nc] != player) break;
            cells.push_back({nc, nr});
        }
        // Negativ irany
        for (int i = 1; i <= 4; i++) {
            int nc = col - dx*i, nr = row - dy*i;
            if (nc < 0 || nc >= _boardSize || nr < 0 || nr >= _boardSize) break;
            if (_board[nr][nc] != player) break;
            cells.push_back({nc, nr});
        }

        if ((int)cells.size() >= 5) {
            _winningCells = std::vector<std::pair<int,int>>(
                cells.begin(), cells.begin() + 5);
            return true;
        }
    }
    return false;
}
