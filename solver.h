//
// Created by lolwuz on 27-Mar-18.
//

#ifndef C4_LOLWUZ_SOLVER_H
#define C4_LOLWUZ_SOLVER_H


#include <array>
#include "c4.h"

struct Threat {
    int x;
    int y;
    int factor;

    Threat(int row, int column, int f){
        x = column;
        y = row;
        factor = f;
    }
};

using Threats = std::vector<Threat>;

class Solver {
    int evaluationTable[6][7] =
    {
        {3, 4, 5 , 7 , 5 , 4, 3},
        {4, 6, 8 , 10, 8 , 6, 4},
        {5, 8, 11, 13, 11, 8, 5},
        {5, 8, 11, 13, 11, 8, 5},
        {4, 6, 8 , 10, 8 , 6, 4},
        {3, 4, 5 , 7 , 5 , 4, 3}
    };

private:
    /*
     * Negamax alpha beta pruning
     * https://en.wikipedia.org/wiki/Negamax
     */
    Move negamax(State board, int depth, int64_t alpha, int64_t beta, const int &color);

    /*
     * Board evaluation
     * Returns a board score for the current player (color)
     */
    int64_t evaluation(const State &board, const int &color);

    int64_t getThreatScore(const State &board, const int &r, const int &c, const int &dR, const int &dC, const Player &player,
                           const Player &opponent);

public:
    /*
     * returns a int that represent a move (column number)
     */
    int getMove(State board, const int &round, const Player &currentPlayer,const int &depth);


};

#endif //C4_LOLWUZ_SOLVER_H
