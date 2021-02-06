#include "../include/bitboard.h"

int BitBoard::pop() {
    u64 temp = bits;

    int count = 0;
    while ((temp & 1) == 0) {
        count++;
        temp >>= 1;
    }

    bits &= (bits-1);
    return count;
}

int BitBoard::count () {
    u64 temp = bits;
    int count;
    for(count = 0; temp; count++, temp &= temp-1);
    return count;
}
