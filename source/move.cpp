//
// Created by moe on 1/28/21.
//

#include "../include/move.h"
#include "../include/utils.h"
#include "../include/consts.h"

Move::Move(Squares from, Squares to, Pieces captured, bool enPassant, bool pawnStart, Pieces promoted, bool castling) :
    from(from), to(to), captured(captured), enPassant(enPassant), pawnStart(pawnStart), promoted(promoted), castling(castling) {
        valid = true;
    }

Move::Move() :
    from(Squares::NO_SQ), to(Squares::NO_SQ), captured(Pieces::EMPTY), enPassant(false), pawnStart(false), promoted(Pieces::EMPTY), castling(false) {
        valid = true;
    }

std::ostream &operator<<(std::ostream &os, const Move &move) {
    if(move.from == Squares::NO_SQ || move.to == Squares::NO_SQ) {
        os << "Invalid Move";
        return os;
    }

    char fileFrom = 'a' + filesBoard.at(move.from);
    int rankFrom = 1 + ranksBoard.at(move.from);

    char fileTo = 'a' + filesBoard.at(move.to);
    int rankTo = 1 + ranksBoard.at(move.to);
    os << fileFrom << rankFrom << fileTo << rankTo <<
        (move.promoted == Pieces::EMPTY ? '\0' : pieceChar[move.promoted]);
    return os;
}

void Move::operator=(const Move& rhs) {
    from = rhs.from;
    to = rhs.to;
    captured = rhs.captured;
    enPassant = rhs.enPassant;
    pawnStart = rhs.pawnStart;
    promoted = rhs.promoted;
    castling = rhs.castling;
    valid = rhs.valid;
    score = rhs.score;
}

Move::Move(const Move& rhs) {
    from = rhs.from;
    to = rhs.to;
    captured = rhs.captured;
    enPassant = rhs.enPassant;
    pawnStart = rhs.pawnStart;
    promoted = rhs.promoted;
    castling = rhs.castling;
    valid = rhs.valid;
    score = rhs.score;
}

bool Move::operator==(const Move& rhs) {
    return (from == rhs.from && to == rhs.to &&
            captured == rhs.captured && enPassant == rhs.enPassant &&
            pawnStart == rhs.pawnStart && promoted == rhs.promoted &&
            castling == rhs.castling && valid == rhs.valid);
}

InvalidMove::InvalidMove() {
    valid = false;
}
