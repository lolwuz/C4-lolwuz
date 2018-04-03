//
// Created by lolwuz on 27-Mar-18.
//

#include "Transposition.h"

unsigned long long int Transposition::computeHash(const State &board) {
    using long long int hash = 0;
    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 7; c++) {
            if(board[r][c] != Player::None){
                int stone = indexOf(board[r][c]);
                hash ^= ZobristTable[r][c][stone];
            }
        }
    }
}

unsigned long long int Transposition::randomInt() {
    std::uniform_int_distribution<unsigned long long int>
            dist(0, UINT64_MAX);
    return dist(mt);
}

int Transposition::indexOf(Player player) {
    switch(player){
        case Player::None:
            return -1;
        case Player::O:
            return 0;
        case Player::X:
            return 1;
    }
}

void Transposition::initTable() {
    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 7; c++) {
            for(int d = 0; d < 3; d++){
                ZobristTable[r][c][d] = randomInt();
            }
        }
    }
}
