//
// Created by lolwuz on 27-Mar-18.
//
#include <climits>
#include <list>
#include "c4.h"
#include "solver.h"

Move Solver::negamax(State board, int depth, int64_t alpha, int64_t beta, const int &color) {
    if (depth == 0 || getWinner(board) != Player::None) {
        return Move {-1, color * evaluation(board, color)};
    }

    int bestValue = -INT_MAX;
    int bestMove = -1;

    for (Move move:getMoves(board)) {
        int value = -negamax(doMove(board, move.first), depth - 1, -beta, -alpha, -color).second;

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

bool Solver::isZugzwang(const State &board, const Player &player) {
    Player otherPlayer = getOtherPlayer(player);

    Threats threatListPlayer = getThreats(board, player);
    Threats threatListOpponent = getThreats(board, otherPlayer);
    int emptySlots = 0; // Number of empty slots on the board

    // Definition of zugzwang
    bool isEven = emptySlots % 2 == 0; // Even amount of empty slots
    int playerOddThreats = 0;
    int opponentOddThreats = 0;

    for (std::pair<int, int> threat : threatListPlayer) {
        bool isThreatEven = (threat.first + 1) % 2 == 0; // Check if threat is in even row
        // 1 Player X has an odd threat with no even threats from Player B
        if (!isThreatEven) {
            // Checks opponents threats
            for (std::pair<int, int> otherThreat : threatListOpponent) {
                bool isOpponentThreatEven = (otherThreat.first + 1) % 2 == 0;
                // 3 Check if threat is in the same column.
                if (otherThreat.second == threat.second) {
                    // 2 Check if no even threat below
                    if (otherThreat.first > threat.first) {
                        playerOddThreats++;
                        return !isOpponentThreatEven; // 4 And Player B had no odd threats in other columns
                    }
                }
                    // Check if B has no odd threats in other columns
                else {
                    if (!isOpponentThreatEven)
                        return false;
                }
            }
        }
    }

    for (std::pair<int, int> opponent : threatListOpponent) {
        bool isThreatEven = (opponent.first + 1) % 2 == 0;

        if (!isThreatEven)
            opponentOddThreats++;
    }
    if (opponentOddThreats < playerOddThreats)
        return true;

    return false;
}

Threats Solver::getThreats(const State &board, const Player &player) {

    Threats threats;

    for(int row = 0; row < 6; row++){
        for(int column = 0; column < 7; column++){
            if(board[row][column] == Player::None){
                if(getWinner(insertThreat(board, player, row, column)) == player){
                    threats.emplace_back(row, column);
                }
            }
        }
    }

    return threats;
}

State Solver::insertThreat(const State &board, const Player &player, const int &row, const int &column){
    State state = board;

    state[row][column] = player;

    return state;
}

int Solver::boundCheck(const State &board, const Player &player, const int &row, const int &column){
    if(row < 0)
        return 0;
    if(row > 5)
        return 0;
    if(column < 0)
        return 0;
    if(column > 6)
        return 0;

    if(board[row][column] == player)
        return 1;
    else
        return 0;
}

int getThreatFactor(const State &board, const int &r, const int &c, const int &dR, const int &dC, const Player &player, const Player &opponent) {
    int factor = 0;
    for(int i = 0; i < 4; i++){
        Player p = board[r + dR * i][c + dC * i];
        if(p == player)
            factor++;
        if(p == opponent)
            return 0;
    }
    return factor;
}

int64_t Solver::evaluation(const State &board, const int &color) {

    int64_t score = 0;

    Player player = color == 1 ? Player::X : Player::O;
    Player opponent = player == Player::X ? Player::O : Player::X;

    int playerThreat = 0;
    int opponentThreat = 0;

    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 7; c++) {
            int playerThreat = 0;
            int opponentThreat = 0;
            if(board[r][c] == Player::None){
                // Check left
                if(c > 2) {
                    playerThreat += getThreatFactor(board, r, c, 0, -1, player, opponent);
                    opponentThreat += getThreatFactor(board, r, c, 0, -1, opponent, player);
                }

                // Check right
                if(c < 4){
                    playerThreat += getThreatFactor(board, r, c, 0, 1, player, opponent);
                    opponentThreat += getThreatFactor(board, r, c, 0, 1, opponent, player);
                }

                // Check Down (Stones drop from the top so up check is not needed)
                if(r < 3){
                    playerThreat += getThreatFactor(board, r, c, -1, 0, player, opponent);
                    opponentThreat += getThreatFactor(board, r, c, -1, 0, opponent, player);
                }

                // Diagonal up
                if(r < 3 && c < 4){
                    playerThreat += getThreatFactor(board, r, c, -1, -1, player, opponent);
                    opponentThreat += getThreatFactor(board, r, c, -1, -1, opponent, player);
                }

                // Diagonal down
                if(r > 2 && c > 2){
                    playerThreat += getThreatFactor(board, r, c, 1, 1, player, opponent);
                    opponentThreat += getThreatFactor(board, r, c, 1, 1, opponent, player);
                }
            }
        }
    }

    Player winner = getWinner(board);

    score = playerThreat - opponentThreat;

    if(winner == player)
        score += INT_MAX;
    if(winner == opponent)
        score -= INT_MAX;


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
