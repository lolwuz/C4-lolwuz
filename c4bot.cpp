// c4bot.cpp
// Aswin van Woudenberg

#include "c4bot.h"

#include <iostream>
#include <sstream>
#include <list>

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
	// Do something more intelligent here instead of returning a random move
	std::cerr << "Calculating move" << std::endl;
	// std::vector<Move> moves = getMoves(state);

	std::cout << "place_disc " << minimax(state, 6).getColumn() << std::endl;
}

Move C4Bot::minimax(State state, int depth){
    Move bestMove {};
	bestMove.setColumn(-1);
	bestMove.setScore(-99999);

    for(int col = 0; col <= 7; col++){
		State newState = doMove(state, col);
		if(state != newState) {
			Move nextMove = minSearch(newState, depth - 1);

			// Evaluation
			if (bestMove.getColumn() == -1 || nextMove.getScore() > bestMove.getScore()) {
				bestMove.setScore(col);
				bestMove.setColumn(nextMove.getColumn());
			}
		}
    }
	// Return the best move
    return bestMove;
}

Move C4Bot::minSearch(State state, int depth){
    Move minMove {};
	minMove.setColumn(-1);
	minMove.setScore(99999);

    for(int col = 0; col <= 7; col++){
		State newState = doMove(state, col);
		if(state != newState){
			Move nextMove  = minimax(newState, depth - 1);

			// Evaluation
			if(minMove.getColumn() == -1 || nextMove.getScore() < minMove.getScore()){
				minMove.setScore(col);
				minMove.setColumn(nextMove.getColumn());
			}
		}
    }
    return minMove;
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

