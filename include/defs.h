//
// Created by moe on 1/23/21.
//

#ifndef TALPAROV_DEFS_H
#define TALPAROV_DEFS_H

#include <iostream>

#define SQUARES 120
#define MAXDEPTH 64
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

enum Pieces : short{
    EMPTY,
    wP, wN, wB, wR, wQ, wK,
    bP, bN, bB, bR, bQ, bK
};

enum Files {
    A, B, C, D,
    E, F, G, H
};

enum Ranks {
    One , Two, Three, Four,
    Five, Six, Seven, Eight
};

enum Side : short {
    White, Black, Both
};

enum SQ : short {
    SQ120, SQ64
};

enum Squares : short {
    A1 = 21, B1, C1, D1, E1, F1, G1, H1,
    A2 = 31, B2, C2, D2, E2, F2, G2, H2,
    A3 = 41, B3, C3, D3, E3, F3, G3, H3,
    A4 = 51, B4, C4, D4, E4, F4, G4, H4,
    A5 = 61, B5, C5, D5, E5, F5, G5, H5,
    A6 = 71, B6, C6, D6, E6, F6, G6, H6,
    A7 = 81, B7, C7, D7, E7, F7, G7, H7,
    A8 = 91, B8, C8, D8, E8, F8, G8, H8,
    NO_SQ, OFF_BOARD
};

enum Castling : short{
    WK = 1, WQ = 2, BK = 4, BQ = 8
};

#endif //TALPAROV_DEFS_H
