//
// Created by lolwuz on 27-Mar-18.
//

#ifndef C4_LOLWUZ_SOLVER_H
#define C4_LOLWUZ_SOLVER_H


#include <array>
#include "c4.h"


/*
 * Negamax alpha beta pruning
 * https://en.wikipedia.org/wiki/Negamax
 */
Move negamax(State board, int depth, int alpha, int beta, const int &color);

/*
 * Board evaluation
 * Returns a board score for the current player (color)
 */
int evaluation(const State &board, const int &color);

int evaluationBoard(const State &board, const int &color);

#endif //C4_LOLWUZ_SOLVER_H
