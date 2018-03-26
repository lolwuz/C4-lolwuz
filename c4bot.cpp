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

    Move move = negamax(6, INT_MIN, INT_MAX, 1);

    std::cout << "place_disc " << move.first << std::endl;
}

std::pair<int, int> C4Bot::negamax(int depth, int a, int b, int sign){
    if(depth <= 0 ) {
        int score = getScore(state);
        return Move(-1, sign * score);
    }

    int bestCol = 3;
    int bestValue = INT_MIN;

    for(Move move:getMoves(state)){

        doMove(state, move.first);

        int value = -negamax(depth - 1, -a, -b, -sign).second;

        undoMove(state, lastMove);

        if(value > bestValue){
            bestValue = value;
            bestCol = move.first;
        }

        a = std::max(value, a);

        if(a >= b)
            break;
    }
    return std::pair<int, int>(bestCol, bestValue);
}

int C4Bot::getScore(State board){
    int score = 0;
    // add individual scores
    for (int y = 0; y < 6; ++y) {
        for (int x = 0; x < 7; ++x) {
            if (state[y][x] == currentPlayer) {
                score++;
            } else if (state[y][x] == otherPlayer) {
                score--;
            }
        }
    }

    // add winner scores
    if (getWinner(state) == currentPlayer) {
        score += 1000000;
    } else if (getWinner(state) == otherPlayer) {
        score -= 1000000;
    }

    // check 2 horizontal
    for (int y = 0; y < 6; ++y) {
        for (int x = 1; x < 7; ++x) {
            if (state[y][x] == state[y][x - 1]) {
                int multi = 0;
                // check if its possible to win from these stones
                if (x + 2 < 7 && state[y][x + 1] == Player::None && state[y][x + 2] == Player::None) {
                    multi = 1;
                }
                if (x - 2 > 0 && x + 2 < 7 && state[y][x - 2] == Player::None && state[y][x + 1] == Player::None) {
                    multi = 1;
                }
                if (x - 3 > 0 && state[y][x - 3] == Player::None && state[y][x - 2] == Player::None) {
                    multi = 1;
                }

                if (state[y][x] == currentPlayer) {
                    score += 100 * multi;
                } else if (state[y][x] == otherPlayer) {
                    score -= 100 * multi;
                }
            }
        }
    }

    // check 3 horizontal
    for (int y = 0; y < 6; ++y) {
        for (int x = 0; x < 5; ++x) {
            if (state[y][x] == state[y][x + 1] && state[y][x] == state[y][x + 2]) {
                int multi = 0;

                // check if its possible to win from these stones
                if (x - 1 > 0 && state[y][x - 1] == Player::None) {
                    multi = 1;
                }
                if (x + 3 < 5 && state[y][x + 3] == Player::None) {
                    multi = 1;
                }

                if (state[y][x] == currentPlayer) {
                    score += 10000 * multi;
                } else if (state[y][x] == otherPlayer) {
                    score -= 10000 * multi;
                }
            }
        }
    }

    // check 2 vertical
    for (int y = 5; y > 0; --y) {
        for (int x = 0; x < 7; ++x) {
            if (state[y][x] == state[y - 1][x]) {
                int multi = 0;
                if (y - 2 > 0) {
                    if (state[y - 2][x] == Player::None) {
                        multi = 1;
                    }
                }

                if (state[y][x] == currentPlayer) {
                    score += 100 * multi;
                } else if (state[y][x] == otherPlayer) {
                    score -= 100 * multi;
                }
            }
        }
    }

    // check 3 vertical
    for (int y = 5; y < 1; --y) {
        for (int x = 0; x < 7; ++x) {
            if (state[y][x] == state[y - 1][x] && state[y][x] == state[y - 2][x]) {
                int multi = 0;
                if (y - 3 > 0) {
                    if (state[y - 3][x] == Player::None) {
                        multi = 1;
                    }
                }

                if (state[y][x] == currentPlayer) {
                    score += 10000 * multi;
                } else if (state[y][x] == otherPlayer) {
                    score -= 10000 * multi;
                }
            }
        }
    }

    // check 2 top left to bottom right
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 4; ++x) {
            if (state[y][x] == state[y + 1][x + 1]) {
                int multi = 0;
                if (y - 2 > 0 && x - 2 > 0 && state[y - 2][x - 2] == Player::None &&
                    state[y - 1][x - 1] == Player::None) {
                    multi = 1;
                }
                if (y - 1 > 0 && x - 1 > 0 && state[y - 1][x - 1] == Player::None &&
                    state[y + 2][x + 2] == Player::None) {
                    multi = 1;
                }
                if (state[y + 2][x + 2] == Player::None && state[y + 3][x + 3] == Player::None) {
                    multi = 1;
                }
                if (state[y][x] == currentPlayer) {
                    score += 100 * multi;
                } else if (state[y][x] == otherPlayer) {
                    score -= 100 * multi;
                }
            }
        }
    }

    // check 3 top left to bottom right
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 4; ++x) {
            if (state[y][x] == state[y + 1][x + 1] && state[y][x] == state[y + 2][x + 2]) {
                int multi = 0;
                if (y - 1 > 0 && x - 1 > 0 && state[y - 1][x - 1] == Player::None) {
                    multi = 1;
                }
                if (state[y + 3][x + 3] == Player::None) {
                    multi = 1;
                }
                if (state[y][x] == currentPlayer) {
                    score += 10000 * multi;
                } else if (state[y][x] == otherPlayer) {
                    score -= 10000 * multi;
                }
            }
        }
    }

    // check 2 bottom left to top right
    for (int y = 5; y > 3; --y) {
        for (int x = 0; x < 4; ++x) {
            if (state[y][x] == state[y - 1][x + 1]) {
                int multi = 0;
                if (y + 2 > 3 && x - 2 > 0 && state[y + 2][x - 2] == Player::None &&
                    state[y + 1][x - 1] == Player::None) {
                    multi = 1;
                }
                if (y + 1 > 3 && x - 1 > 0 && state[y + 1][x - 1] == Player::None &&
                    state[y - 2][x + 2] == Player::None) {
                    multi = 1;
                }
                if (state[y - 2][x + 2] == Player::None && state[y - 3][x + 3] == Player::None) {
                    multi = 1;
                }
                if (state[y][x] == currentPlayer) {
                    score += 100 * multi;
                } else if (state[y][x] == otherPlayer) {
                    score -= 100 * multi;
                }
            }
        }
    }

    // check 3 bottom left to top right
    for (int y = 5; y > 3; --y) {
        for (int x = 0; x < 4; ++x) {
            if (state[y][x] == state[y - 1][x + 1] && state[y][x] == state[y - 2][x + 2]) {
                int multi = 0;
                if (y + 1 > 3 && x - 1 > 0 && state[y + 1][x - 1] == Player::None) {
                    multi = 1;
                }
                if (state[y - 3][x + 3] == Player::None) {
                    multi = 1;
                }

                if (state[y][x] == currentPlayer) {
                    score += 10000 * multi;
                } else if (state[y][x] == otherPlayer) {
                    score -= 10000 * multi;
                }
            }
        }
    }
    
    return score;
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

