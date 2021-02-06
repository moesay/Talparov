#ifndef TALPAROV_BITBOARD_H
#define TALPAROV_BITBOARD_H

#include "../include/defs.h"

class BitBoard {
private:
    u64 bits;

public:
    int pop();
    int count();
    void set(int);
    void unset(int);
    void print() const;
    BitBoard& operator=(u64);
};

#endif //TALPAROV_BITBOARD_H

