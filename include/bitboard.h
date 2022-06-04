//
// Created by moe on 1/27/21.
//

#ifndef TALPAROV_BITBOARD_H
#define TALPAROV_BITBOARD_H

#include "../include/consts.h"

class BitBoard {
private:
    u64 bits;

public:
    int pop();
    int count() const;
    void set(int);
    void unset(int);
    void print() const;
    BitBoard& operator=(u64);
};

#endif //TALPAROV_BITBOARD_H