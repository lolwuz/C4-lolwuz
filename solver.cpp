//
// Created by lolwuz on 27-Mar-18.
//
#include <climits>
#include <list>
#include <stdint.h>
#include "c4.h"
#include "solver.h"

Move Solver::negamax(State board, int depth, int64_t alpha, int64_t beta, const int &color) {
    if (depth == 0 || getWinner(board) != Player::None) {
        return Move {-1, evaluation(board, color)};
    }

    int64_t bestValue = -INT64_MAX;
    int bestMove = -1;

    for (Move move:getMoves(board)) {
        int64_t value = -negamax(doMove(board, move.column), depth - 1, -beta, -alpha, -color).score;

        if (value > bestValue) {
            bestMove = move.column;
            bestValue = value;
        }

        if (value > alpha) {
            alpha = value;
        }

        if (alpha >= beta)
            break;
    }
    return {bestMove, bestValue};
}

bool Solver::isInsideBound(int row, int column){
    return column >= 0 && column <= 6 && row >= 0 && row <= 5;
}

int Solver::getThreatFactor(const State &board, const int &r, const int &c, const int &dR, const int &dC,
                            const Player &player, const Player &opponent) {
    int factor = 0;

    for (int i = 1; i < 4; i++) {
        if (!isInsideBound(r + dR * i, c + dC * i))
            break;
        Player p = board[r + dR * i][c + dC * i];
        if (p == player)
            factor++;
        if (factor == 3) // Max factor reached.
            return factor;
        if (p == opponent) // Factor not possible
            break;
    }

    for (int i = -1; i > -4; i--) {
        if (!isInsideBound(r + dR * i, c + dC * i))
            break;

        Player p = board[r + dR * i][c + dC * i];
        if (p == player)
            factor++;
        if (factor == 3)
            return factor;
        if (p == opponent)
            return -1;
    }

    return factor;
}



Threats Solver::getThreats(const State &board, const Player &player, const Player &opponent){
    Threats threats;

    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 7; c++) {
            if(board[r][c] == Player::None){
                int horizontalFactor = getThreatFactor(board, r, c, 0, 1, player, opponent);
                int verticalFactor = getThreatFactor(board, r, c, -1, 0, player, opponent);
                int diagonalFactor = getThreatFactor(board, r, c, 1, 1, player, opponent);

                int factor = std::max(horizontalFactor, std::max(verticalFactor, diagonalFactor));

                threats.emplace_back(r, c, factor);
            }
        }
    }

    return threats;
}

void Solver::filterThreats(Threats &playerThreats, Threats &opponentThreats) {
    for (auto &playerThreat : playerThreats) {
        if(!playerThreat.isEven){}
    }
}

int64_t Solver::evaluation(const State &board, const int &color) {
    // Base score
    int64_t score = 0;

    // Get maximizing player
    Player player = color == 1 ? Player::X : Player::O;
    Player opponent = player == Player::X ? Player::O : Player::X;

    // Return max for winning/losing board
    Player winner = getWinner(board);
    if(winner == player)
        return INT64_MAX;
    else if(winner == opponent)
        return -INT64_MAX;

    // Get threats for each player
    Threats playerThreats = getThreats(board, player, opponent);
    Threats opponentThreats = getThreats(board, opponent, player);

    filterThreats(playerThreats, opponentThreats);

    for(auto threat: playerThreats){
        switch(threat.factor){
            case 0:
                score += evaluationTable[threat.r][threat.c];
            case 1:
                score += 100;
                break;
            case 2:
                score += 1000;
                break;
            case 3:
                score += 10000;
                break;
            default:
                break;
        }
    }

    for(auto threat: opponentThreats){
        switch(threat.factor){
            case 0:
                score -= evaluationTable[threat.r][threat.c];
            case 1:
                score -= 100;
                break;
            case 2:
                score -= 1000;
                break;
            case 3:
                score -= 10000;
                break;
            default:
                break;
        }
    }

    return score;
}

int Solver::getMove(State board, const int &round, const Player &currentPlayer, const int &depth) {
    // evaluation(board, -1);

    int column = 0; // Middle column

    if (round < 2)
        column = 3;
    else if (currentPlayer == Player::X)
        column = negamax(board, depth, -INT64_MAX, INT64_MAX, 1).column;
    else // Playing as Player O
        column = negamax(board, depth, -INT64_MAX, INT64_MAX, -1).column;

    if (column == -1)
        return getMoves(board).front().column;
    else
        return column;
}
