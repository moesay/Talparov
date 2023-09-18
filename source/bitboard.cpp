//
// Created by moe on 1/27/21.
//

#include "../include/bitboard.h"
#include "../include/utils.h"
#include <iostream>

int BitBoard::pop() {
    std::uint64_t temp = bits;
    int count = 0;
    while ((temp & 1) == 0) {
        count++;
        temp >>= 1;
    }
    bits &= (bits-1);
    return count;
}

int BitBoard::count () const {
    std::uint64_t temp = bits;
    int count = 0;
    for(count; temp; count++, temp &= temp-1);
    return count;
}

void BitBoard::print() const {
    Squares sq; int sq64;
    for(int rank = Ranks::Eight; rank >= Ranks::One; --rank) {
        for(int file = Files::A; file <= Files::H; ++file) {
            sq = FRtoSq((Files)file, (Ranks)rank);
            sq64 = getSquareIndex(SQ::SQ64, sq);

            (1ull << sq64) & bits ? std::cout << "X" : std::cout << "-";
        }
        std::cout << "\n";
    }
}

void BitBoard::unset(int index) {
    bits &= ~(1ull << getSquareIndex(SQ::SQ64, index));
}

void BitBoard::set(int index) {
    bits |= 1ull << getSquareIndex(SQ::SQ64, index);
}

BitBoard& BitBoard::operator=(std::uint64_t val) {
    bits = val;
    return *this;
}

std::uint64_t BitBoard::getBitmask() const {
    return bits;
}