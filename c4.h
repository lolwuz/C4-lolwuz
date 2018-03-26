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
using Move = std::pair<int, int>;
using State = std::array<std::array<Player,7>,6>;

// used to get a random element from a container
template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}

std::ostream &operator<<(std::ostream& os, const Player &p);
std::ostream &operator<<(std::ostream& os, const State &s);
Player getCurrentPlayer(const State &state);
void doMove(State &state, int &column);
void undoMove(State &state, Move lastMove);
Player getWinner(const State &state);
Player getOtherPlayer(Player me);
std::vector<Move> getMoves(const State &state);

#endif // C4_H

