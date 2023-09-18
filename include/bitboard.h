//
// Created by moe on 1/27/21.
//

#ifndef TALPAROV_BITBOARD_H
#define TALPAROV_BITBOARD_H

#include "../include/defs.h"

class BitBoard {
private:
    std::uint64_t bits;

public:
    std::uint64_t getBitmask() const;
    int pop();
    int count() const;
    void set(int);
    void unset(int);
    void print() const;
    BitBoard& operator=(std::uint64_t);
};

#endif //TALPAROV_BITBOARD_H