// c4.h
// Aswin van Woudenberg

#ifndef C4_H
#define C4_H

#include <array>
#include <vector>
#include <ctime>
#include <random>
#include <iterator>
#include <iostream>

enum class Player { None, X, O };

struct Move {
    int column;
    int64_t score;

    Move(int c, int64_t s){
        column = c;
        score = s;
    }
};

struct Threat {
    int c;
    int r;
    int factor;
    bool isEven;

    Threat(int row, int column, int f){
        c = column;
        r = row;
        factor = f;
        isEven = (r + 1) % 2 == 0;
    }
};

using Threats = std::vector<Threat>;

using State = std::array<std::array<Player,7>,6>;

std::ostream &operator<<(std::ostream& os, const Player &p);
std::ostream &operator<<(std::ostream& os, const State &s);
Player getCurrentPlayer(const State &state);
State doMove(const State &state, const int &m);
Player getWinner(const State &state);
Player getOtherPlayer(Player me);
std::vector<Move> getMoves(const State &state);

#endif // C4_H

