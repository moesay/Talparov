//
// Created by moe on 1/23/21.
//

#ifndef TALPAROV_UTILS_H
#define TALPAROV_UTILS_H

#include "defs.h"
#include <array>
#include <chrono>
#include <random>
#include <assert.h>
#include "consts.h"

#define int_to_ms(x) std::chrono::milliseconds(x)

constexpr long operator""_MB (unsigned long long val) { return val * 1024 * 1024; }

using time_type = std::chrono::time_point<std::chrono::system_clock>;

Pieces& operator++(Pieces&);
Pieces operator++(Pieces&, int);


extern std::array<int, SQUARES> filesBoard;
extern std::array<int, SQUARES> ranksBoard;

extern std::array<int, SQUARES> sq120ToSq64;
extern std::array<int, 64> sq64ToSq120;

extern std::uint64_t piecesHK[13][120];
extern std::uint64_t sideHK;
extern std::uint64_t castleHK[16];
extern std::uint64_t whitePassersMask[64];
extern std::uint64_t blackPassersMask[64];
extern std::uint64_t isolatedPawnsMask[64];
extern std::uint64_t fileBBMask[8];
extern std::uint64_t rankBBMask[8];

unsigned long long getRand64();

void initIndices();
void initFRBoards();
void initEvaluationMasks();
void initAll();
Squares mirrorSquare64(Squares);

void generateHashKeys();
bool isItKnight(const Pieces&);
bool isItRookOrQueen(const Pieces&);
bool isItBishopOrQueen(const Pieces&);
bool isItKing(const Pieces&);
bool isItPawn(const Pieces&);

Squares FRtoSq(Files, Ranks);
int getSquareIndex(SQ, int);
void printBB(std::uint64_t);
int getCPULoad();

time_type getTime();
std::string timeToStr(std::chrono::time_point<std::chrono::system_clock>& t);
std::ostream& operator<<(std::ostream& os, std::chrono::time_point<std::chrono::system_clock>& t);
#endif //TALPAROV_UTILS_H
