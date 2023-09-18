//
// Created by moe on 1/28/21.
//

#ifndef TALPAROV_MOVE_H
#define TALPAROV_MOVE_H

#include "../include/defs.h"

class Move{
public:
    Squares from, to;
    Pieces captured;
    bool enPassant;
    bool pawnStart;
    Pieces promoted;
    bool castling;
    bool valid;

    int score;

    Move();
    Move(Squares from, Squares to,
         Pieces captured = Pieces::EMPTY, bool enPassant = false, bool pawnStart = false, Pieces promoted = Pieces::EMPTY, bool castling = false);

    friend std::ostream& operator<<(std::ostream&, const Move&);
    Move(const Move&);
    bool operator==(const Move&);
    void operator=(const Move&);
    char* getMoveStr();
};

class InvalidMove : public Move {
public:
    InvalidMove();
};

#endif //TALPAROV_MOVE_H