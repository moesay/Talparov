//
// Created by moe on 1/24/21.
//

#ifndef TALPAROV_CONSTS_H
#define TALPAROV_CONSTS_H

#include "defs.h"
#include <array>

extern const char pieceChar[];
extern const char sideChar[];
extern const char rankChar[];
extern const char fileChar[];

extern const int pieceBig[];
extern const int pieceMaj[];
extern const int pieceMin[];
extern const int pieceVal[];
extern const int pieceColor[];

extern const int pieceKnight[];
extern const int pieceKing[];
extern const int piecePawn[];
extern const int pieceRookQueen[];
extern const int pieceBishopQueen[];

extern const int knDir[8];
extern const int rkDir[4];
extern const int biDir[4];
extern const int kiDir[8];

extern const int castleHList[];
extern const int mirror[64];

extern const int EndgameMatch;
extern const std::array<int, 64> kingEndgameTable;
extern const std::array<int, 64> kingOpeningTable;
extern const std::array<int, 64> pawnScoreTable;
extern const std::array<int, 64> knightScoreTable;
extern const std::array<int, 64> bishopScoreTable;
extern const std::array<int, 64> rookScoreTable;
extern const std::array<int, 64> queenScoreTable;
extern const std::array<int, 64> blackSq;
extern const int isolatedPawn;
extern const int bishopPairScore;
extern const int passedPawn[];
extern const int rookOpenFile;
extern const int rookSemiOpenFile;
extern const int queenOpenFile;
extern const int queenSemiOpenFile;
extern const int pawnIslandScore;

#endif //TALPAROV_CONSTS_H
