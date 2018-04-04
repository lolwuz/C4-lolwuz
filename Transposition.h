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

private:
    unsigned long long int ZobristTable[6][7][3];
    std::mt19937 mt;

    /*
     * Generates Random int in range 0 to INT64_MAX
     */
    unsigned long long int randomInt();

    int indexOf(Player player);



    unsigned long long int computeHash(const State &board);

public:
    /*
     * Constructor initializes the Hash function.
     */
    Transposition();

    /*
     * Push a Hash entry into the hash-table
     */
    void push(Hashentry);

    /*
     * Get hashentry from hash 'code'
     */
    Hashentry get(const State &board);
};

#endif //C4_LOLWUZ_TRANSPOSITION_H
