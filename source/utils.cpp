//
// Created by moe on 1/23/21.
//
#include <array>
#include "../include/utils.h"
#include <chrono>
#include <ratio>
#include <iomanip>
#include <string>
#include <iterator>
#include <fstream>

std::array<int, SQUARES> sq120ToSq64;
std::array<int, 64> sq64ToSq120;

std::array<int, SQUARES> filesBoard;
std::array<int, SQUARES> ranksBoard;

std::uint64_t whitePassersMask[64];
std::uint64_t blackPassersMask[64];
std::uint64_t isolatedPawnsMask[64];

//for indexing
std::uint64_t fileBBMask[8];
std::uint64_t rankBBMask[8];

std::uint64_t piecesHK[13][120];
std::uint64_t sideHK;
std::uint64_t castleHK[16];

time_type getTime() {
    /* return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(); */
    return std::chrono::system_clock::now();
}

void printBB(std::uint64_t bits) {
    Squares sq; int sq64;
    for(int rank = Ranks::Eight; rank >= Ranks::One; --rank) {
        for(int file = Files::A; file <= Files::H; ++file) {
            sq = FRtoSq((Files)file, (Ranks)rank);
            sq64 = getSquareIndex(SQ::SQ64, sq);

            (1ull << sq64) & bits ? std::cout << "X" : std::cout << "-";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void initEvaluationMasks() {
    int square, tempSq;
    for(int i = 0; i < 8; i++) {
        fileBBMask[i] = 0ull;
        rankBBMask[i] = 0ull;
    }

    for(int rank = Ranks::Eight; rank >= Ranks::One; rank--) {
        for(int file = Files::A; file <= Files::H; file++) {
            square = rank * 8 + file;
            fileBBMask[file] |= (1ull << square);
            rankBBMask[rank] |= (1ull << square);
        }
    }

    for(int i = 0; i < 64; i++) {
        whitePassersMask[i] = 0ull;
        blackPassersMask[i] = 0ull;
        isolatedPawnsMask[i] = 0ull;
    }

    for(square = 0; square < 64; square++) {

        tempSq = square + 8;
        while(tempSq < 64) {
            whitePassersMask[square] |= (1ull << tempSq);
            tempSq += 8;
        }

        tempSq = square - 8;;
        while(tempSq >= 0) {
            blackPassersMask[square] |= (1ull << tempSq);
            tempSq -= 8;
        }

        if(filesBoard.at(getSquareIndex(SQ::SQ120, square)) > Files::A) {
            isolatedPawnsMask[square] |= fileBBMask[filesBoard.at(getSquareIndex(SQ::SQ120, square)) - 1];

            tempSq = square + 7;
            while(tempSq < 64) {
                whitePassersMask[square] |= (1ull << tempSq);
                tempSq += 8;
            }

            tempSq = square - 9;
            while(tempSq >= 0) {
                blackPassersMask[square] |= (1ull << tempSq);
                tempSq -= 8;
            }
        }

        if(filesBoard.at(getSquareIndex(SQ::SQ120, square)) < Files::H) {
            isolatedPawnsMask[square] |= fileBBMask[filesBoard.at(getSquareIndex(SQ::SQ120, square)) + 1];

            tempSq = square + 9;
            while(tempSq < 64) {
                whitePassersMask[square] |= (1ull << tempSq);
                tempSq += 8;
            }

            tempSq = square - 7;
            while(tempSq >= 0) {
                blackPassersMask[square] |= (1ull << tempSq);
                tempSq -= 8;
            }
        }
    }
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
    initEvaluationMasks();
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

std::string squareToText(Squares sq) {
    char file = 'a' + filesBoard.at(sq);
    char rank = '1' + ranksBoard.at(sq);

    std::string ret = std::string() + file+ rank;
    return ret;
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

Squares mirrorSquare64(Squares square) {
    return (Squares) mirror[square];
}

std::string timeToStr(time_type& t) {
    std::time_t time = std::chrono::system_clock::to_time_t(t);
    std::string time_str = std::ctime(&time);
    uint64_t ms = (t.time_since_epoch().count() / 1000000) % 1000;

    time_str = time_str.substr(11, 8);
    return (time_str + "." + std::to_string(ms));
}

std::ostream& operator<<(std::ostream& os, std::chrono::time_point<std::chrono::system_clock>& t) {
    os << timeToStr(t);
    return os;
}
#ifdef __unix__
/*
 * This function is not safe at all.
 * It depends on the format of the stat file without checking whatsoever
 *
 * I don't care tho. I will fix it later.
 */
int getCPULoad() {
    std::ifstream is("/proc/stat");
    std::istream_iterator<std::string> iter(is);
    std::vector<std::string> vec;
    std::copy_n(++iter, 10, std::back_inserter(vec));

    return (std::stoi(vec[3]) * 100) /
        std::accumulate(vec.begin(), vec.end(), 0, [](int t, const std::string &str) { return t + std::stoll(str); });
}
#else
int getCPULoad() { return 0; }
#endif
