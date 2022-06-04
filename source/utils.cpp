//
// Created by moe on 1/23/21.
//
#include <array>
#include "../include/utils.h"
#include <chrono>
#include <ratio>

std::array<int, SQUARES> sq120ToSq64;
std::array<int, 64> sq64ToSq120;

std::array<int, SQUARES> filesBoard;
std::array<int, SQUARES> ranksBoard;

u64 piecesHK[13][120];
u64 sideHK;
u64 castleHK[16];

uint64_t getTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

Squares FRtoSq(Files file, Ranks rank) {
    return Squares(
            ((21+file) + (rank * 10))
    );
}

//sq -> square type to be returned
int getSquareIndex(SQ sq, int index) {
    if(sq == SQ::SQ120) {
        return sq64ToSq120.at(index);
    }
    else if(sq == SQ::SQ64) {
        return sq120ToSq64.at(index);
    }
    return -1;
}

void initAll() {
    generateHashKeys();
    initIndices();
    initFRBoards();
}

void initIndices() {
    int square, square64 = 0;
    for(auto &i : sq120ToSq64)
        i = 65;
    for(auto &i : sq64ToSq120)
        i = 120;

    for(int rank = Ranks::One; rank <= Ranks::Eight; rank++) {
        for(int file = Files::A; file <= Files::H; file++) {
            square = FRtoSq((Files)file, (Ranks)rank);
            sq64ToSq120.at(square64) = square;
            sq120ToSq64.at(square) = square64;
            square64++;
        }
    }
}

void generateHashKeys() {
    for(int i = 0; i < 13; i++) {
        for (int j = 0; j < 120; j++) {
            piecesHK[i][j] = getRand64();
        }
    }

    for(int i = 0; i < 16; i++) {
        castleHK[i] = getRand64();
    }
        sideHK = getRand64();
}

unsigned long long getRand64() {
    std::random_device device;
    std::default_random_engine generator(device());

    std::uniform_int_distribution<unsigned long long> dist(0, 0xFFFFFFFFFFFFFFFF);

    return dist(generator);
}

void initFRBoards() {
    Squares tSq;
    for(auto &i : filesBoard)
        i = Squares::OFF_BOARD;

    for(auto &i : ranksBoard)
        i = Squares::OFF_BOARD;
    for(int rank = Ranks::One; rank <= Ranks::Eight; rank++) {
        for(int file = Files::A; file <= Files::H; file++) {
            tSq = FRtoSq((Files)file, (Ranks)rank);
            filesBoard.at(tSq) = file;
            ranksBoard.at(tSq) = rank;
        }
    }
}

Pieces& operator++(Pieces &piece) {
    int t = static_cast<int>(piece);
    t++;
    piece = static_cast<Pieces>(t);

    return piece;
}

Pieces operator++(Pieces &piece, [[maybe_unused]] int temp) {
    ++piece;
    return piece;
}

bool isItKnight(const Pieces& piece) {
    return pieceKnight[piece];
}

bool isItRookOrQueen(const Pieces& piece) {
    return pieceRookQueen[piece];
}

bool isItBishopOrQueen(const Pieces& piece) {
    return pieceBishopQueen[piece];
}
bool isItKing(const Pieces& piece) {
    return pieceKing[piece];
}

bool isItPawn(const Pieces &piece) {
    return piecePawn[piece];
}
