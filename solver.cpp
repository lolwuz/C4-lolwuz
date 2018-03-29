//
// Created by lolwuz on 27-Mar-18.
//
#include <climits>
#include <list>
#include "c4.h"
#include "solver.h"

Move Solver::negamax(State board, int depth, int64_t alpha, int64_t beta, const int &color){
    if(depth == 0 || getWinner(board) != Player::None){
        return Move { -1, evaluation(board, color)};
    }

    int bestValue = -INT_MAX;
    int bestMove = -1;

    for(Move move:getMoves(board)){
        int value = -negamax(doMove(board, move.first), depth - 1, - beta, - alpha, - color).second;

        if(value > bestValue){
            bestMove = move.first;
            bestValue = value;
        }

        if(value > alpha) {
            alpha = value;
        }

        if(alpha >= beta)
            break;
    } return Move{bestMove, bestValue};
}

bool Solver::isZugzwang(const State &board, const Player &player) {
    Player otherPlayer = getOtherPlayer(player);

    std::list<std::pair<int, int>> threatListPlayer; // Stores possible winning moves for the player
    std::list<std::pair<int, int>> threatListOpponent; // Stores possible winning moves for the opponent
    int emptySlots = 0; // Number of empty slots on the board

    // Iterate trough the board to find winning connections (threats)
    for(Move move: getMoves(board)){
        State getBoard = doMove(board, move.first);
        if(getWinner(getBoard) != Player::None){
            for(int row = 0; row < 6; row++){
                if(board[row][move.first] != Player::None){
                    if(board[row][move.first] == player){
                        threatListPlayer.push_front({row, move.first});
                    } else {
                        threatListOpponent.push_front({row, move.first});
                    }
                }
            }
        }
    }

    // Definition of zugzwang
    bool isEven = emptySlots % 2 == 0; // Even amount of empty slots
    int playerOddThreats = 0;
    int opponentOddThreats = 0;

    for (std::pair<int, int> threat : threatListPlayer) {
        bool isThreatEven = (threat.first + 1) % 2 == 0; // Check if threat is in even row
        // 1 Player X has an odd threat with no even threats from Player B
        if(!isThreatEven){
            // Checks opponents threats
            for(std::pair<int, int> otherThreat : threatListOpponent){
                bool isOpponentThreatEven = (otherThreat.first + 1) % 2 == 0;
                // 3 Check if threat is in the same column.
                if(otherThreat.second == threat.second){
                    // 2 Check if no even threat below
                    if(otherThreat.first > threat.first){
                        playerOddThreats++;
                        return !isOpponentThreatEven; // 4 And Player B had no odd threats in other columns
                    }
                }
                // Check if B has no odd threats in other columns
                else {
                    if(!isOpponentThreatEven)
                        return false;
                }
            }
        }
    }

    for (std::pair<int, int> opponent : threatListOpponent) {
        bool isThreatEven = (opponent.first + 1) % 2 == 0;

        if(!isThreatEven)
            opponentOddThreats++;
    }
    if(opponentOddThreats < playerOddThreats)
        return true;

    return false;
}

int Solver::evaluationBoard(const State &board, const int &color){
    int score = 0;
    Player player;
    if(color == 1)
        player = Player::X;
    else
        player = Player::O;

    if(getWinner(board) == player)
        return INT_MAX;

    for(int row = 0; row < 6; row++){
        for(int column = 0; column < 6; column++){
            if(board[row][column] == player) {
                score += getPositionScore(board, player, row, column);
            }
        }
    }

    return score;
}

int Solver::getPositionScore(const State &board, const Player &player, const int &row, const int &column){
    int score = 1;

    return score;
}

bool Solver::isOutOfBounds(const int &row, const int &column){
    if(row < 0)
        return true;
    if(row > 5)
        return true;
    if(column < 0)
        return true;
    return column > 6;
}


int Solver::evaluation(const State &board, const int &color) {
    Player player;
    if(color == 1)
        player = Player::X;
    else
        player = Player::O;



    int score = 0;
    // add individual scores
    for (int y = 0; y < 6; ++y) {
        for (int x = 0; x < 7; ++x) {
            if (board[y][x] == player) {
                score+= evaluationTable[y][x];
            } else if (board[y][x] == getOtherPlayer(player)) {
                score-= evaluationTable[y][x];
            }
        }
    }

    // Check for Zugzwang
    if (isZugzwang(board, player)) {
        score += 1000000;
    }
    if (isZugzwang(board, getOtherPlayer(player))){
        score -= 1000000;
    }

    // add winner scores
    if (getWinner(board) == player) {
        score += 1000000;
    } else if (getWinner(board) == getOtherPlayer(player)) {
        score -= 1000000;
    }


    // check 2 horizontal
    for (int y = 0; y < 6; ++y) {
        for (int x = 1; x < 7; ++x) {
            if (board[y][x] == board[y][x - 1]) {
                int multi = 0;
                // check if its possible to win from these stones
                if (x + 2 < 7 && board[y][x + 1] == Player::None && board[y][x + 2] == Player::None) {
                    multi = 1;
                }
                if (x - 2 > 0 && x + 2 < 7 && board[y][x - 2] == Player::None && board[y][x + 1] == Player::None) {
                    multi = 1;
                }
                if (x - 3 > 0 && board[y][x - 3] == Player::None && board[y][x - 2] == Player::None) {
                    multi = 1;
                }

                if (board[y][x] == player) {
                    score += 100 * multi;
                } else if (board[y][x] == getOtherPlayer(player)) {
                    score -= 100 * multi;
                }
            }
        }
    }

    // check 3 horizontal
    for (int y = 0; y < 6; ++y) {
        for (int x = 0; x < 5; ++x) {
            if (board[y][x] == board[y][x + 1] && board[y][x] == board[y][x + 2]) {
                int multi = 0;

                // check if its possible to win from these stones
                if (x - 1 > 0 && board[y][x - 1] == Player::None) {
                    multi = 1;
                }
                if (x + 3 < 5 && board[y][x + 3] == Player::None) {
                    multi = 1;
                }

                if (board[y][x] == player) {
                    score += 10000 * multi;
                } else if (board[y][x] == getOtherPlayer(player)) {
                    score -= 10000 * multi;
                }
            }
        }
    }

    // check 2 vertical
    for (int y = 5; y > 0; --y) {
        for (int x = 0; x < 7; ++x) {
            if (board[y][x] == board[y - 1][x]) {
                int multi = 0;
                if (y - 2 > 0) {
                    if (board[y - 2][x] == Player::None) {
                        multi = 1;
                    }
                }

                if (board[y][x] == player) {
                    score += 100 * multi;
                } else if (board[y][x] == getOtherPlayer(player)) {
                    score -= 100 * multi;
                }
            }
        }
    }

    // check 3 vertical
    for (int y = 5; y < 1; --y) {
        for (int x = 0; x < 7; ++x) {
            if (board[y][x] == board[y - 1][x] && board[y][x] == board[y - 2][x]) {
                int multi = 0;
                if (y - 3 > 0) {
                    if (board[y - 3][x] == Player::None) {
                        multi = 1;
                    }
                }

                if (board[y][x] == player) {
                    score += 10000 * multi;
                } else if (board[y][x] == getOtherPlayer(player)) {
                    score -= 10000 * multi;
                }
            }
        }
    }

    // check 2 top left to bottom right
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 4; ++x) {
            if (board[y][x] == board[y + 1][x + 1]) {
                int multi = 0;
                if (y - 2 > 0 && x - 2 > 0 && board[y - 2][x - 2] == Player::None &&
                    board[y - 1][x - 1] == Player::None) {
                    multi = 1;
                }
                if (y - 1 > 0 && x - 1 > 0 && board[y - 1][x - 1] == Player::None &&
                    board[y + 2][x + 2] == Player::None) {
                    multi = 1;
                }
                if (board[y + 2][x + 2] == Player::None && board[y + 3][x + 3] == Player::None) {
                    multi = 1;
                }
                if (board[y][x] == player) {
                    score += 100 * multi;
                } else if (board[y][x] == getOtherPlayer(player)) {
                    score -= 100 * multi;
                }
            }
        }
    }

    // check 3 top left to bottom right
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 4; ++x) {
            if (board[y][x] == board[y + 1][x + 1] && board[y][x] == board[y + 2][x + 2]) {
                int multi = 0;
                if (y - 1 > 0 && x - 1 > 0 && board[y - 1][x - 1] == Player::None) {
                    multi = 1;
                }
                if (board[y + 3][x + 3] == Player::None) {
                    multi = 1;
                }
                if (board[y][x] == player) {
                    score += 10000 * multi;
                } else if (board[y][x] == getOtherPlayer(player)) {
                    score -= 10000 * multi;
                }
            }
        }
    }

    // check 2 bottom left to top right
    for (int y = 5; y > 3; --y) {
        for (int x = 0; x < 4; ++x) {
            if (board[y][x] == board[y - 1][x + 1]) {
                int multi = 0;
                if (y + 2 > 3 && x - 2 > 0 && board[y + 2][x - 2] == Player::None &&
                    board[y + 1][x - 1] == Player::None) {
                    multi = 1;
                }
                if (y + 1 > 3 && x - 1 > 0 && board[y + 1][x - 1] == Player::None &&
                    board[y - 2][x + 2] == Player::None) {
                    multi = 1;
                }
                if (board[y - 2][x + 2] == Player::None && board[y - 3][x + 3] == Player::None) {
                    multi = 1;
                }
                if (board[y][x] == player) {
                    score += 100 * multi;
                } else if (board[y][x] == getOtherPlayer(player)) {
                    score -= 100 * multi;
                }
            }
        }
    }

    // check 3 bottom left to top right
    for (int y = 5; y > 3; --y) {
        for (int x = 0; x < 4; ++x) {
            if (board[y][x] == board[y - 1][x + 1] && board[y][x] == board[y - 2][x + 2]) {
                int multi = 0;
                if (y + 1 > 3 && x - 1 > 0 && board[y + 1][x - 1] == Player::None) {
                    multi = 1;
                }
                if (board[y - 3][x + 3] == Player::None) {
                    multi = 1;
                }

                if (board[y][x] == player) {
                    score += 10000 * multi;
                } else if (board[y][x] == getOtherPlayer(player)) {
                    score -= 10000 * multi;
                }
            }
        }
    }

    return score;
}

int Solver::getMove(State board, const int &round, const Player &currentPlayer, const int &depth) {
    int column = 0; // Middle column

    if(round < 2)
        column = 3;
    else
        if (currentPlayer == Player::X)
            column = negamax(board, depth, -INT64_MAX, INT64_MAX, 1).first;
        else // Playing as Player O
            column = negamax(board, depth, -INT64_MAX, INT64_MAX, -1).first;

    if(column == -1)
        return getMoves(board).front().first;
    else
        return column;
}
