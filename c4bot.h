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
    Move lastMove;
    // Table used for evalution scores
    int evaluationTable[6][7] = {
            {3, 4, 5, 7, 5, 4, 3},
            {4, 6, 8, 10, 8, 6, 4},
            {5, 8, 11, 13, 11, 8, 5},
            {5, 8, 11, 13, 11, 8, 5},
            {4, 6, 8, 10, 8, 6, 4},
            {3, 4, 5, 7, 5, 4, 3}
    };


    State state = {{{{Player::None}}}};

    std::vector<std::string> split(const std::string &s, char delim);

    std::pair<int, int> negamax(State board, int depth, int color);

    int getScore(State board, int color);

    int eval(State board, int color);

    int scorePosition(Player currentPlayer, State board, int row, int column, int y, int x);

    bool isOutOfBounds(int row, int col);

    void setting(std::string &key, std::string &value);

    void update(std::string &key, std::string &value);

    void move(int timeout);

public:
    void run();
};

#endif // C4BOT_H

