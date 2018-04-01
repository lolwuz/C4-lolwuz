//
// Created by lolwuz on 27-Mar-18.
//
#include <climits>
#include <list>
#include "c4.h"
#include "solver.h"

Move Solver::negamax(State board, int depth, int64_t alpha, int64_t beta, const int &color) {
    if (depth == 0 || getWinner(board) != Player::None) {
        return Move {-1, evaluation(board, color)};
    }

    int64_t bestValue = -INT_MAX;
    int bestMove = -1;

    for (Move move:getMoves(board)) {
        int64_t value = -negamax(doMove(board, move.first), depth - 1, -beta, -alpha, -color).second;

        if (value > bestValue) {
            bestMove = move.first;
            bestValue = value;
        }

        if (value > alpha) {
            alpha = value;
        }

        if (alpha >= beta)
            break;
    }
    return Move{bestMove, bestValue};
}

int64_t Solver::getThreatScore(const State &board, const int &r, const int &c, const int &dR, const int &dC, const Player &player, const Player &opponent) {
    int factor = 0;

    for(int i = 0; i < 4; i++){
        Player p = board[r + dR * i][c + dC * i];
        if(p == player)
            factor++;
        if(p == opponent)
            return 0;
    }


    return ipow(10, factor);
}

int64_t Solver::evaluation(const State &board, const int &color) {

    int64_t score = 0;

    Player player = color == 1 ? Player::X : Player::O;
    Player opponent = player == Player::X ? Player::O : Player::X;

    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 7; c++) {
            if(board[r][c] != Player::None){
                // Check left
                if(c > 2) {
                    score += getThreatScore(board, r, c, 0, -1, player, opponent);
                    score -= getThreatScore(board, r, c, 0, -1, opponent, player);
                }

                // Check right
                if(c < 4){
                    score += getThreatScore(board, r, c, 0, 1, player, opponent);
                    score -= getThreatScore(board, r, c, 0, 1, opponent, player);
                }

                // Check Down (Stones drop from the top so up check is not needed)
                if(r < 3){
                    score += getThreatScore(board, r, c, -1, 0, player, opponent);
                    score -= getThreatScore(board, r, c, -1, 0, opponent, player);
                }

                // Diagonal up
                if(r < 3 && c < 4){
                    score += getThreatScore(board, r, c, -1, -1, player, opponent);
                    score -= getThreatScore(board, r, c, -1, -1, opponent, player);
                }

                // Diagonal down
                if(r > 2 && c > 2){
                    score += getThreatScore(board, r, c, 1, 1, player, opponent);
                    score -= getThreatScore(board, r, c, 1, 1, opponent, player);
                }
            }
        }
    }

    return score;
}

int Solver::getMove(State board, const int &round, const Player &currentPlayer, const int &depth) {

    int column = 0; // Middle column

    if (round < 2)
        column = 3;
    else if (currentPlayer == Player::X)
        column = negamax(board, depth, -INT64_MAX, INT64_MAX, 1).first;
    else // Playing as Player O
        column = negamax(board, depth, -INT64_MAX, INT64_MAX, -1).first;

    if (column == -1)
        return getMoves(board).front().first;
    else
        return column;
}
