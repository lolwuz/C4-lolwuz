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

    if(round < 2)
        std::cout << "place_disc " << 3 << std::endl;
    else
        if(currentPlayer == Player::X)
            std::cout << "place_disc " << negamax(state, 8, -INT_MAX, INT_MAX, 1).first << std::endl;
        else
            std::cout << "place_disc " << negamax(state, 8, -INT_MAX, INT_MAX, -1).first << std::endl;
}

Move C4Bot::negamax(State board, int depth, int alpha, int beta, const int &color){
    if(depth == 0 || getWinner(board) != Player::None){
        return Move {-1 , eval(board, color)};
    }

    int bestValue = -INT_MAX;
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

int C4Bot::eval(const State &board, const int &color) {
    Player player;
    if(color == 1)
        player = Player::X;
    else
        player = Player::O;

    int score = 0;
    // add individual scores
    for (int y = 0; y < 6; ++y) {
        for (int x = 0; x < 7; ++x) {
            if (board[y][x] == player) {
                score++;
            } else if (board[y][x] == getOtherPlayer(player)) {
                score--;
            }
        }
    }

    // add winner scores
    if (getWinner(board) == player) {
        score += 1000000;
    } else if (getWinner(board) == getOtherPlayer(player)) {
        score -= 1000000;
    }

    // check 2 horizontal
    for (int y = 0; y < 6; ++y) {
        for (int x = 1; x < 7; ++x) {
            if (board[y][x] == board[y][x - 1]) {
                int multi = 0;
                // check if its possible to win from these stones
                if (x + 2 < 7 && board[y][x + 1] == Player::None && board[y][x + 2] == Player::None) {
                    multi = 1;
                }
                if (x - 2 > 0 && x + 2 < 7 && board[y][x - 2] == Player::None && board[y][x + 1] == Player::None) {
                    multi = 1;
                }
                if (x - 3 > 0 && board[y][x - 3] == Player::None && board[y][x - 2] == Player::None) {
                    multi = 1;
                }

                if (board[y][x] == player) {
                    score += 100 * multi;
                } else if (board[y][x] == getOtherPlayer(player)) {
                    score -= 100 * multi;
                }
            }
        }
    }

    // check 3 horizontal
    for (int y = 0; y < 6; ++y) {
        for (int x = 0; x < 5; ++x) {
            if (board[y][x] == board[y][x + 1] && board[y][x] == board[y][x + 2]) {
                int multi = 0;

                // check if its possible to win from these stones
                if (x - 1 > 0 && board[y][x - 1] == Player::None) {
                    multi = 1;
                }
                if (x + 3 < 5 && board[y][x + 3] == Player::None) {
                    multi = 1;
                }

                if (board[y][x] == player) {
                    score += 10000 * multi;
                } else if (board[y][x] == getOtherPlayer(player)) {
                    score -= 10000 * multi;
                }
            }
        }
    }

    // check 2 vertical
    for (int y = 5; y > 0; --y) {
        for (int x = 0; x < 7; ++x) {
            if (board[y][x] == board[y - 1][x]) {
                int multi = 0;
                if (y - 2 > 0) {
                    if (board[y - 2][x] == Player::None) {
                        multi = 1;
                    }
                }

                if (board[y][x] == player) {
                    score += 100 * multi;
                } else if (board[y][x] == getOtherPlayer(player)) {
                    score -= 100 * multi;
                }
            }
        }
    }

    // check 3 vertical
    for (int y = 5; y < 1; --y) {
        for (int x = 0; x < 7; ++x) {
            if (board[y][x] == board[y - 1][x] && board[y][x] == board[y - 2][x]) {
                int multi = 0;
                if (y - 3 > 0) {
                    if (board[y - 3][x] == Player::None) {
                        multi = 1;
                    }
                }

                if (board[y][x] == player) {
                    score += 10000 * multi;
                } else if (board[y][x] == getOtherPlayer(player)) {
                    score -= 10000 * multi;
                }
            }
        }
    }

    // check 2 top left to bottom right
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 4; ++x) {
            if (board[y][x] == board[y + 1][x + 1]) {
                int multi = 0;
                if (y - 2 > 0 && x - 2 > 0 && board[y - 2][x - 2] == Player::None &&
                    board[y - 1][x - 1] == Player::None) {
                    multi = 1;
                }
                if (y - 1 > 0 && x - 1 > 0 && board[y - 1][x - 1] == Player::None &&
                    board[y + 2][x + 2] == Player::None) {
                    multi = 1;
                }
                if (board[y + 2][x + 2] == Player::None && board[y + 3][x + 3] == Player::None) {
                    multi = 1;
                }
                if (board[y][x] == player) {
                    score += 100 * multi;
                } else if (board[y][x] == getOtherPlayer(player)) {
                    score -= 100 * multi;
                }
            }
        }
    }

    // check 3 top left to bottom right
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 4; ++x) {
            if (board[y][x] == board[y + 1][x + 1] && board[y][x] == board[y + 2][x + 2]) {
                int multi = 0;
                if (y - 1 > 0 && x - 1 > 0 && board[y - 1][x - 1] == Player::None) {
                    multi = 1;
                }
                if (board[y + 3][x + 3] == Player::None) {
                    multi = 1;
                }
                if (board[y][x] == player) {
                    score += 10000 * multi;
                } else if (board[y][x] == getOtherPlayer(player)) {
                    score -= 10000 * multi;
                }
            }
        }
    }

    // check 2 bottom left to top right
    for (int y = 5; y > 3; --y) {
        for (int x = 0; x < 4; ++x) {
            if (board[y][x] == board[y - 1][x + 1]) {
                int multi = 0;
                if (y + 2 > 3 && x - 2 > 0 && board[y + 2][x - 2] == Player::None &&
                    board[y + 1][x - 1] == Player::None) {
                    multi = 1;
                }
                if (y + 1 > 3 && x - 1 > 0 && board[y + 1][x - 1] == Player::None &&
                    board[y - 2][x + 2] == Player::None) {
                    multi = 1;
                }
                if (board[y - 2][x + 2] == Player::None && board[y - 3][x + 3] == Player::None) {
                    multi = 1;
                }
                if (board[y][x] == player) {
                    score += 100 * multi;
                } else if (board[y][x] == getOtherPlayer(player)) {
                    score -= 100 * multi;
                }
            }
        }
    }

    // check 3 bottom left to top right
    for (int y = 5; y > 3; --y) {
        for (int x = 0; x < 4; ++x) {
            if (board[y][x] == board[y - 1][x + 1] && board[y][x] == board[y - 2][x + 2]) {
                int multi = 0;
                if (y + 1 > 3 && x - 1 > 0 && board[y + 1][x - 1] == Player::None) {
                    multi = 1;
                }
                if (board[y - 3][x + 3] == Player::None) {
                    multi = 1;
                }

                if (board[y][x] == player) {
                    score += 10000 * multi;
                } else if (board[y][x] == getOtherPlayer(player)) {
                    score -= 10000 * multi;
                }
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

