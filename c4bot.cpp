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

    Move move;
    if(currentPlayer == Player::X)
        move = negamax(state, 6, 1);
    else
        move = negamax(state, 6, -1);

    std::cout << "place_disc " << move.first << std::endl;
}

Move C4Bot::negamax(State board, int depth, int color){
    if(depth <= 0 || getWinner(board) != Player::None){
        return { -1, color * getScore(board, color)};
    }

    int bestMove = -1;
    int bestScore = INT_MIN;

    for(Move move:getMoves(state)){
        int nextScore = -negamax(doMove(board, move.first), depth - 1, -color).second;

        if(move.first == 6){
            // Debug
        }
        if(nextScore >= bestScore) {
            bestScore = nextScore;
            bestMove = move.first;
        }
    }

    return Move{bestMove, bestScore};
}

int C4Bot::getScore(State board, int color){
    int evaluationTable[6][7] = {
		{3, 4, 5, 7, 5, 4, 3},
		{4, 6, 8, 10, 8, 6, 4},
		{5, 8, 11, 13, 11, 8, 5},
		{5, 8, 11, 13, 11, 8, 5},
		{4, 6, 8, 10, 8, 6, 4},
		{3, 4, 5, 7, 5, 4, 3}
	};


	Player checkPlayer;
	if(color == 1)
		checkPlayer = Player::X;
	else
		checkPlayer = Player::O;

    int sum = 0;
    for (int rows = 0; rows < 6; rows++) {
		for (int columns = 0; columns < 7; columns++) {
			if (board[rows][columns] == checkPlayer) {
				sum += evaluationTable[rows][columns];

				for (int next = 0; next < 4; next++) {
					if (!isOutOfBounds(rows - next, columns))
						if (board[rows - next][columns] == checkPlayer) // Down
							sum += evaluationTable[rows - next][columns] * evaluationTable[rows][columns];
					if(!isOutOfBounds(rows + next, columns))
						if (board[rows + next][columns] == checkPlayer) // Up
							sum += evaluationTable[rows + next][columns] * evaluationTable[rows][columns];

					if(!isOutOfBounds(rows + next, columns + next))
						if (board[rows + next][columns + next] == checkPlayer) // Dia 1
							sum += evaluationTable[rows + next][columns + next] * evaluationTable[rows][columns];

					if(!isOutOfBounds(rows - next, columns - next))
						if (board[rows - next][columns - next] == checkPlayer) // Dia 2
							sum += evaluationTable[rows - next][columns - next] * evaluationTable[rows][columns];

					if(!isOutOfBounds(rows, columns - next))
						if (board[rows][columns - next] == checkPlayer) // Left
							sum += evaluationTable[rows][columns - next] * evaluationTable[rows][columns];

					if(!isOutOfBounds(rows, columns + next))
						if (board[rows][columns + next] == checkPlayer) // Right
							sum += evaluationTable[rows][columns + next] * evaluationTable[rows][columns];
				}
			}
		}
	}
    return sum;
}

bool C4Bot::isOutOfBounds(int row, int col){
	if(row < 0)
		return true;
	if(row > 5)
		return true;
	if(col < 0)
		return true;
	if(col > 6)
		return true;
	return false;
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
	} else if (key == "field_columns") {
		field_columns = std::stoi(value);
	} else if (key == "field_rows") {
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

