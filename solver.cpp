//
// Created by lolwuz on 27-Mar-18.
//
#include <climits>
#include "c4.h"
#include "solver.h"

Move negamax(State board, int depth, int alpha, int beta, const int &color){
    if(depth == 0 || getWinner(board) != Player::None){
        return Move {-1 , evaluation(board, color)};
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

int evaluationBoard(const State &board, const int &color){
    Player player;
    if(color == 1)
        player = Player::X;
    else
        player = Player::O;

    for(int row = 0; row < 6; row++){
        for(int column = 0; column < 6; column++){
            if(board[row][column] == player)
                return 0;
        }
    }
}


int evaluation(const State &board, const int &color) {
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
                score++;
            } else if (board[y][x] == getOtherPlayer(player)) {
                score--;
            }
        }
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
