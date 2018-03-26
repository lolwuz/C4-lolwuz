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

	// std::vector<Move> moves = getMoves(state);

    if(currentPlayer == Player::X) {
        std::cout << "place_disc " << negamax(state, 6, 1).first << std::endl;
    }
    else {
        std::cout << "place_disc " << negamax(state, 6, -1).first << std::endl;
    }
}

Move C4Bot::negamax(State board, int depth, int color){
    if(depth <= 0 || getWinner(board) != Player::None){
        return { -1, color * eval(board, color)};
    }

    int bestMove = -1;
    int bestScore = INT_MIN;

    for(Move move:getMoves(state)){
        int nextScore = -negamax(doMove(board, move.first), depth - 1, -color).second;

        if(nextScore >= bestScore) {
            bestScore = nextScore;
            bestMove = move.first;
        }
    }

    return Move{bestMove, bestScore};
}

int C4Bot::eval(State board, int color) {
    Player checkPlayer;
    if(color == 1)
        checkPlayer = Player::X;
    else
        checkPlayer = Player::O;

    int sum = 0;

    if(getWinner(board) == checkPlayer){
        return INT_MAX;
    }

    int points = 0;
    int verticalPoints = 0;
    int horizontalPoints = 0;
    int diagonalPoints1 = 0;
    int diagonalPoints2 = 0;

    // Vertical
    for(int row = 0; row < field_rows - 3; row++){
        for(int column = 0; column < field_columns; column++){
            int score = scorePosition(checkPlayer, board, row, column, 1, 0);
            verticalPoints += score;
        }
    }

    // Horizontal
    for(int row = 0; row < field_rows; row++){
        for(int column = 0; column < field_columns - 3; column++){
            int score = scorePosition(checkPlayer, board, row, column, 0, 1);
            horizontalPoints += score;
        }
    }

    // Diagonal 1
    for(int row = 0; row < field_rows - 3; row++){
        for(int column = 0; column < field_columns - 3; column++){
            int score = scorePosition(checkPlayer, board, row, column, 1, 1);
            diagonalPoints1 += score;
        }
    }

    // Diagonal 2
    for(int row = 3; row < field_rows; row++){
        for(int column = 0; column < field_columns - 3; column++){
            int score = scorePosition(checkPlayer, board, row, column, -1, 1);
            diagonalPoints2 += score;
        }
    }

    points = diagonalPoints1 + diagonalPoints2 + horizontalPoints + verticalPoints;
    return points;
}

int C4Bot::scorePosition(Player currentPlayer, State board, int row, int column, int y, int x) {
    int points = 0;

    for (int i = 0; i < 4; i++) {
        if (board[row][column] == currentPlayer){
            points++;
        }
        row += y;
        column += x;
    }
    return points;
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

