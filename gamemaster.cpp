#include "gamemaster.hpp"
#include <cmath>
#include <random>

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

// Egymas melletti azonos szinu kovek szama az adott iranyban (mindket oldal)
int GameMaster::countLine(int col, int row, int dx, int dy, int player) const {
    int count = 0;
    for (int i = 1; i <= 4; i++) {
        int nc = col + dx*i, nr = row + dy*i;
        if (nc < 0 || nc >= _boardSize || nr < 0 || nr >= _boardSize) break;
        if (_board[nr][nc] != player) break;
        count++;
    }
    for (int i = 1; i <= 4; i++) {
        int nc = col - dx*i, nr = row - dy*i;
        if (nc < 0 || nc >= _boardSize || nr < 0 || nr >= _boardSize) break;
        if (_board[nr][nc] != player) break;
        count++;
    }
    return count;
}

// Heurisztikus gepi lepesvalasztas (pontozas alapjan, difficulty 1-3)
std::pair<int,int> GameMaster::computerMove(int difficulty) {
    static std::mt19937 rng(std::random_device{}());

    int me  = _currentPlayer;
    int opp = (me == 1) ? 2 : 1;

    static const int dirs[4][2] = {{1,0},{0,1},{1,1},{1,-1}};

    // Jitter tartomany nehezsegtol fuggoen
    int jitter = 0;
    if      (difficulty == 1) jitter = 500;
    else if (difficulty == 2) jitter = 30;

    std::vector<std::pair<int,int>> candidates;
    int bestScore = -1;

    for (int r = 0; r < _boardSize; r++) {
        for (int c = 0; c < _boardSize; c++) {
            if (_board[r][c] != 0) continue;

            int score = 0;

            for (auto& d : dirs) {
                int myLine  = countLine(c, r, d[0], d[1], me);
                int oppLine = countLine(c, r, d[0], d[1], opp);

                if (myLine  >= 4) score += 1000000;
                if (oppLine >= 4) score +=  100000;
                if (myLine  == 3) score +=   10000;
                if (oppLine == 3) score +=    5000;
                if (myLine  == 2) score +=     200;
                if (oppLine == 2) score +=     100;
                if (myLine  == 1) score +=      20;
            }

            // Szomszedsagi bonus (2 cella korzetben)
            for (int dr = -2; dr <= 2; dr++)
                for (int dc = -2; dc <= 2; dc++) {
                    if (dr == 0 && dc == 0) continue;
                    int nr = r + dr, nc = c + dc;
                    if (nr >= 0 && nr < _boardSize && nc >= 0 && nc < _boardSize
                        && _board[nr][nc] != 0)
                        score += 5;
                }

            // Kozepponti preferencia
            int dist = std::abs(c - _boardSize/2) + std::abs(r - _boardSize/2);
            score += std::max(0, 10 - dist);

            // Veletlenszeru zaj (difficulty 1: nagy, 2: kis, 3: nincs)
            if (jitter > 0)
                score += std::uniform_int_distribution<int>(-jitter, jitter)(rng);

            if (score > bestScore) {
                bestScore = score;
                candidates.clear();
                candidates.push_back({c, r});
            } else if (difficulty == 3 && score == bestScore) {
                candidates.push_back({c, r});
            }
        }
    }

    // Nehez mod: az osszes legjobb pontszamu cella kozul veletlenszeruen valaszt
    if (difficulty == 3 && candidates.size() > 1) {
        return candidates[std::uniform_int_distribution<int>(0, (int)candidates.size()-1)(rng)];
    }
    return candidates.empty() ? std::make_pair(_boardSize/2, _boardSize/2) : candidates[0];
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
