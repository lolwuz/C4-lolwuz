//
// Created by lolwuz on 27-Mar-18.
//

#ifndef C4_LOLWUZ_TRANSPOSITION_H
#define C4_LOLWUZ_TRANSPOSITION_H

#include "c4.h"

#include <random>
#include <limits>

enum class Flag { UPPERBOUND, LOWERBOUND, EXACT };

struct Hashentry {
    unsigned long long int zobris{};
    int depth{};
    int ancient{};
    Flag flag{};
    Move move = move; // Contains evaluation
};

class Transposition {
    unsigned long long int ZobristTable[6][7][3];
    std::mt19937 mt;

public:
    unsigned long long int randomInt();

    int indexOf(Player player);

    void initTable();

    unsigned long long int computeHash(const State &board);
};

#endif //C4_LOLWUZ_TRANSPOSITION_H
