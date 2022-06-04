//
// Created by moe on 1/23/21.
//

#ifndef TALPAROV_UTILS_H
#define TALPAROV_UTILS_H

#include <array>
#include <random>
#include <assert.h>
#include "consts.h"

Pieces& operator++(Pieces&); //work
Pieces operator++(Pieces&, int);


extern std::array<int, SQUARES> filesBoard;
extern std::array<int, SQUARES> ranksBoard;

extern std::array<int, SQUARES> sq120ToSq64;
extern std::array<int, 64> sq64ToSq120;

extern u64 piecesHK[13][120];
extern u64 sideHK;
extern u64 castleHK[16];
unsigned long long getRand64();

void initIndices();
void initFRBoards();
void initAll();

void generateHashKeys();
bool isItKnight(const Pieces&);
bool isItRookOrQueen(const Pieces&);
bool isItBishopOrQueen(const Pieces&);
bool isItKing(const Pieces&);
bool isItPawn(const Pieces&);

Squares FRtoSq(Files, Ranks);
int getSquareIndex(SQ, int);

uint64_t getTime();
#endif //TALPAROV_UTILS_H
