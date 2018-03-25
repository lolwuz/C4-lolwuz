// c4bot.h
// Aswin van Woudenberg

#ifndef C4BOT_H
#define C4BOT_H

#include <string>
#include <vector>

#include "c4.h"

class C4Bot {
    int timebank;
    int time_per_move;
    std::string player_names[2];
    std::string your_bot;
    int your_botid;
    int field_columns;
    int field_rows;

    int i = 0;

    int round;
    State state = {{{{Player::None}}}};

    std::vector<std::string> split(const std::string &s, char delim);

    /*
     * Function determines the best move by minimax.
     * https://nl.wikipedia.org/wiki/Minimax
     */
    Move minimax(State state, int depth);

    Move minSearch(State state, int depth);

    int getScore(State state);

    void setting(std::string &key, std::string &value);

    void update(std::string &key, std::string &value);

    void move(int timeout);

public:
    void run();
};

#endif // C4BOT_H

