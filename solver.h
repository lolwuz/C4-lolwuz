//
// Created by lolwuz on 27-Mar-18.
//

#ifndef C4_LOLWUZ_SOLVER_H
#define C4_LOLWUZ_SOLVER_H


#include <array>
#include "c4.h"


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

    /*
     * Return the factor of a threat. 3 == Only one stone needed to connect 4. 2 == 2 stones are needed.
     */
    int getThreatFactor(const State &board, const int &r, const int &c, const int &dR, const int &dC, const Player &player, const Player &opponent);

    /*
     * Looks for all possible threats (empty squares that can connect 4) on the board.
     */
    Threats getThreats(const State &board, const Player &player, const Player &opponent);

    /*
     * There are other possible combinations of threats for the two players. Let us suppose the combi-
     * nations of one threat per player, assuming that the threats are in different columns. We can now easily
     * deduce whose threat will be the strongest (we assume that no other threats will be created).
     * http://www.informatik.uni-trier.de/~fernau/DSL0607/Masterthesis-Viergewinnt.pdf (Page 20).
     */
    void filterThreats(Threats &playerThreats, Threats &opponentThreats);

    /*
     * Array boundary filter
     */
    bool isInsideBound(int row, int column);

public:
    /*
     * returns a int that represent a move (column number)
     */
    int getMove(State board, const int &round, const Player &currentPlayer,const int &depth);
};

#endif //C4_LOLWUZ_SOLVER_H
