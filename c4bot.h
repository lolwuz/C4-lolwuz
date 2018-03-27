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
    int round;
    Player currentPlayer;
    Player otherPlayer;


    State state = {{{{Player::None}}}};

    std::vector<std::string> split(const std::string &s, char delim);

    Move negamax(State board, int depth, int alpha, int beta, int color);

    int eval(State board, int color);

    int scorePosition(Player currentPlayer, State board, int row, int column, int y, int x);

    bool isOutOfBounds(int row, int color);

    void setting(std::string &key, std::string &value);

    void update(std::string &key, std::string &value);

    void move(int timeout);

public:
    void run();
};

#endif // C4BOT_H

