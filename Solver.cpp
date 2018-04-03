//
// Created by lolwuz on 27-Mar-18.
//
#include <climits>
#include <list>
#include "c4.h"
#include "Solver.h"

Move Solver::negamax(State board, int depth, int64_t alpha, int64_t beta, const int &color) {
    int64_t alphaOrigin = alpha;

    Hashentry ttEntry = transposition.lookup();

    if(ttEntry.depth > depth){
        if (ttEntry.flag == Flag::EXACT)
            return { -1 ,ttEntry.move.score};
        else if(ttEntry.flag == Flag::LOWERBOUND)
            alpha = std::max(alpha, ttEntry.move.score);
        else if (ttEntry.flag == Flag::UPPERBOUND){
            beta = std::min(beta, ttEntry.move.score);
        }

        if(alpha > beta)
            return {-1, ttEntry.move.score };
    }

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

    // Transposition Table storing
    ttEntry.move.score = bestValue;
    if(bestValue < alphaOrigin)
        ttEntry.flag = Flag::UPPERBOUND;
    else if(bestValue > beta)
        ttEntry.flag = Flag::LOWERBOUND;
    else
        ttEntry.flag = Flag::EXACT;

    ttEntry.depth = depth;

    transposition.store(board, ttEntry);

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
    int opponentOdd = 0;
    int playerOdd = 0;
    for (auto &playerThreat : playerThreats) {
        if(playerThreat.isEven){

        }
        else {
            for(auto &opponentThreat : opponentThreats){
                // even threats from Player B below it in the same column,
                if(opponentThreat.isEven && opponentThreat.c == playerThreat.c && opponentThreat.r < playerThreat.r){
                    playerThreat.factor = -1;
                } else { // If instead Player A had a greater number of odd threats (with no even threats from Player B below them)
                    opponentOdd++;
                }
            }

            playerOdd++;
        }
    }

    if(playerOdd > opponentOdd){
        for(auto &opponentThreat : opponentThreats){
            if(opponentThreat.factor == 3){
                opponentThreat.factor = -1;
            }
        }
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

    // filterThreats(playerThreats, opponentThreats);

    for(auto threat: playerThreats){
        switch(threat.factor){
            case 0:
                score += 10;
                break;
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
                score -= 10;
                break;
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
    // Generate the transposition Table
    transposition.initTable();

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
