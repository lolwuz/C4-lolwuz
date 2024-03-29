// c4.cpp
// Aswin van Woudenberg

#include "c4.h"

std::ostream &operator<<(std::ostream& os, const Player &p) {
	if (p == Player::None) {
		os << ".";
	} else if (p == Player::X) {
		os << "X";
	} else if (p == Player::O) {
		os << "O";
	}
	return os;
}

std::ostream &operator<<(std::ostream& os, const State &s) {
	for (int r=0; r<6; r++) {
		for (int c=0; c<7; c++) {
			os << s[r][c];
		}
		os << std::endl;
	}
	return os;
}

Player getCurrentPlayer(const State &state)
{
    int countX = 0;
    int countO = 0;
    for (int r=0; r<6; r++) {
		for (int c=0; c<7; c++) {
        	if (state[r][c] == Player::X) countX++;
        	else if(state[r][c] == Player::O) countO++;
		}
    }
    return (countX > countO ? Player::O : Player::X); 
}

int ipow(int base, int exp)
{
    int result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}

State doMove(const State &state, const int &m)
{
    State result = state;
    for (int r=0; r<6; r++) {
        if (r == 5 || result[r+1][m] != Player::None) {
            result[r][m] = getCurrentPlayer(state);
            return result;
        }
    }
    return result; // Invalid move
}

void undoMove(State &state, Move lastMove){
    for(int rows = 0; rows < 6; rows++){
        if(state[rows][lastMove.column] == Player::None){
            state[rows - 1][lastMove.column] = Player::None;
        }
        if(rows == 5)
            state[rows][lastMove.column] = Player::None;
    }
}

Player getWinner(const State &state)
{
	for (int r=0; r<6; r++) {
		for (int c=0; c<7; c++) {
			if (state[r][c] != Player::None) {
				if (c<4) {
					if (state[r][c] == state[r][c+1] && state[r][c] == state[r][c+2] && state[r][c] == state[r][c+3])
						return state[r][c];
					if (r<3) {
						if (state[r][c] == state[r+1][c+1] && state[r][c] == state[r+2][c+2] && state[r][c] == state[r+3][c+3])
							return state[r][c];
					}
				}
				if (r<3) {
					if (state[r][c] == state[r+1][c] && state[r][c] == state[r+2][c] && state[r][c] == state[r+3][c])
						return state[r][c];
					if (c>2) {
						if (state[r][c] == state[r+1][c-1] && state[r][c] == state[r+2][c-2] && state[r][c] == state[r+3][c-3])
							return state[r][c];
					}
				}
			}

		}
	}
    return Player::None;
}

std::vector<Move> getMoves(const State &state) {
    std::vector<Move> moves;
    if (getWinner(state) == Player::None) {
        // Move exploration order from the middle column
        if (state[0][3] == Player::None)
            moves.emplace_back(3, 0);

        for (int i = 1; i < 4; i++) {
            if (state[0][3 + i] == Player::None)
                moves.emplace_back(3 + i, 0);
            if (state[0][3 - i] == Player::None)
                moves.emplace_back(3 - i, 0);
        }
    }

    return moves;
}

Player getOtherPlayer(Player me){
    if(me == Player::X)
        return Player::O;
    else
        return Player::X;
}


