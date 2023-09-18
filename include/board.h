//
// Created by moe on 1/23/21.
//

#ifndef TALPAROV_BOARD_H
#define TALPAROV_BOARD_H

#include "defs.h"
#include <array>
#include <tuple>
#include <type_traits>
#include "../include/utils.h"
#include "../include/consts.h"
#include "../include/bitboard.h"
#include "../include/move.h"
#include "tptable.h"


struct Undo {
public:
    Move move;
    Squares enPas;
    int castlePerm;
    int fiftyMoves;
    std::uint64_t posKey;
};

class Board {
private:
    friend class MoveGenerator;
    friend class Search;
    std::array<Pieces, SQUARES> pieces;
    std::array<BitBoard , 3> pawns;
    std::array<Squares, 2> kingSq;
    std::array<Undo, 2048> history;
    Side side;
    Squares enPas;
    int fiftyMoves;
    int hisPly;
    unsigned int castlePerm;
    std::uint64_t posKey;
    std::array<int, 2> bigPieces;
    std::array<int, 2> majPieces;
    std::array<int, 2> minPieces;
    std::array<int, 2> material;
    std::array<std::array<int, 10>, 13> pList;
    bool materialDraw() const;
    int searchHistory[13][SQUARES];
    Move killers[2][MAXDEPTH];

    bool isSqOffBoard(const Squares&) const;
    int getPvLine(const int&);
    void makeNullMove();
    void undoNullMove();

public:
    std::array<int, 13> piecesNum;
    int ply;
    bool check() const;
    int getFromPiecesList(Pieces, int) const;
    int& setPiecesList(Pieces, int);
    void reset();
    void print() const;
    void updateMaterials();
    bool loadFEN(std::string);
    void hashPiece(Pieces, Squares);
    std::string squareToText(Squares) const;
    void clearPiece(Squares);
    void addPiece(Squares, Pieces);
    void mirror();
    void movePiece(const Move&);
    bool makeMove(const Move&);
    bool isRep() const;
    void undoMove();
    void hashCastling();
    Side getSide() const;
    void hashSide();
    void hashEnPassant();
    std::uint64_t generateHashKey() const;
    void allAttackedSquares(Side) const;
    Side getOppSide();
    std::uint64_t getKey() const;
    int eval() const;
    TranspositionTable tpTable;
    std::array<Move, MAXDEPTH> pvArray;
    const std::tuple<bool, int> isSquareAttacked(Squares, Side) const;
    const bool isSquareAttackedOptm(Squares, Side) const;
};


#endif //TALPAROV_BOARD_H