// c4bot.cpp
// Aswin van Woudenberg

#include "c4bot.h"

#include <iostream>
#include <sstream>
#include <list>
#include <climits>

void C4Bot::run() {
	std::string line;
	while (std::getline(std::cin, line)) {
		std::vector<std::string> command = split(line, ' ');
		if (command[0] == "settings") {
			setting(command[1], command[2]);
		} else if (command[0] == "update" && command[1] == "game") {
			update(command[2], command[3]);
		} else if (command[0] == "action" && command[1] == "move") {
			move(std::stoi(command[2]));
		} else {
			std::cerr << "Unknown command: " << line << std::endl;
		}
	}
}

void C4Bot::move(int timeout) {

    currentPlayer = getCurrentPlayer(state);
    otherPlayer = getOtherPlayer(currentPlayer);

	// Do something more intelligent here instead of returning a random move
    if(currentPlayer == Player::X)
        std::cout << "place_disc " << negamax(state, 9, -INT_MAX, INT_MAX, 1).first << std::endl;
    else
        std::cout << "place_disc " << negamax(state, 9, -INT_MAX, INT_MAX, -1).first << std::endl;


}

Move C4Bot::negamax(State board, int depth, int alpha, int beta, int color){
    if(depth == 0 || getWinner(board) != Player::None){
        return Move {-1 ,color * eval(board, color)};
    }

    int bestValue = INT_MIN;
    int bestMove = -1;

    for(Move move:getMoves(state)){
        int value = -negamax(doMove(board, move.first), depth - 1, - beta, - alpha, - color).second;

        if(value > bestValue){
            bestMove = move.first;
            bestValue = value;
        }

        if(value > alpha) {
            bestMove = move.first;
            alpha = value;
        }

        if(alpha >= beta)
            break;
    } return Move{bestMove, bestValue};
}

int C4Bot::eval(State board, int color) {
    Player player;
    if(color == 1){
        player = Player::O;
    } else {
        player = Player::X;
    }


    int score = 0;
    for (int row = 0; row < 6; row++) {
        for (int column = 0; column < 7; column++) {
            if (board[row][column] == player) {
                // Dia 1
                score += scorePosition(player, board, row, column, 1, 1);

                // Dia 2
                score += scorePosition(player, board, row, column, -1, -1);

                // Horizontal
                score += scorePosition(player, board, row, column, 0, 1);
                score += scorePosition(player, board, row, column, 0, -1);

                // Vertical
                score += scorePosition(player, board, row, column, 1, 0);
                score += scorePosition(player, board, row, column, -1, 0);

            } else if (board[row][column] == Player::None)
                break;
            else {
                // Dia 1
                score -= scorePosition(player, board, row, column, 1, 1);

                // Dia 2
                score -= scorePosition(player, board, row, column, -1, -1);

                // Horizontal
                score -= scorePosition(player, board, row, column, 0, 1);
                score -= scorePosition(player, board, row, column, 0, -1);

                // Vertical
                score -= scorePosition(player, board, row, column, 1, 0);
                score -= scorePosition(player, board, row, column, -1, 0);
            }
        }
    }

    return score;
}

int C4Bot::scorePosition(Player player, State board, int row, int column, int y, int x) {
    int points = 0;
    points += evaluationTable[row][column];

    for (int i = 0; i < 4; i++) {
        if(!isOutOfBounds(row, column)){
            if (board[row][column] == player){
                points = points * evaluationTable[row][column];
            }
            row += y;
            column += x;
        }
    }
    return points;
}

bool C4Bot::isOutOfBounds(const int &row, const int &column){
    if(row < 0)
        return true;
    else if(row > 5)
        return true;
    else if(column > 6)
        return true;
    else return column < 0;
}

void C4Bot::update(std::string &key, std::string &value) {
	if (key == "round") {
		round = std::stoi(value);
	} else if (key == "field") {
		int row = 0;
		int col = 0;
		std::vector<std::string> fields = split(value, ',');
		for (std::string &field : fields) {
			if (field == "0") {
				state[row][col] = Player::X; 
			} else if (field == "1") {
				state[row][col] = Player::O;
			} else {
				state[row][col] = Player::None;
			}
			col++;
			if (col == 7) {
				row++; 
				col = 0;
			}
		}
	}
}

void C4Bot::setting(std::string &key, std::string &value) {
	if (key == "timebank") {
		timebank = std::stoi(value);
	} else if (key == "time_per_move") {
		time_per_move = std::stoi(value);
	} else if (key == "player_names") {
		std::vector<std::string> names = split(value, ',');
		player_names[0] = names[0];
		player_names[1] = names[1];
	} else if (key == "your_bot") {
		your_bot = value;
	} else if (key == "your_botid") {
		your_botid = std::stoi(value);
	} else if (key == "field_width") {
		field_columns = std::stoi(value);
	} else if (key == "field_height") {
		field_rows = std::stoi(value);
	}
}

std::vector<std::string> C4Bot::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

