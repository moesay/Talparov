//
// Created by moe on 1/24/21.
//
#include "../include/consts.h"

constexpr char pieceChar[] = ".PNBRQKpnbrqk";
constexpr char sideChar[] = "wb-";
constexpr char rankChar[] = "12345678";
constexpr char fileChar[] = "abcdefgh";

constexpr int pieceBig[13] = {false, false, true, true, true, true, true, false, true, true, true, true, true};
constexpr int pieceMaj[13] = {false, false, false, false, true, true, true, false, false, false, true, true, true};
constexpr int pieceMin[13] = {false, false, true, true, false, false, false, false, true, true, false, false, false};
constexpr int pieceVal[13] = {0, 100, 325, 325, 550, 1000, 50000, 100, 325, 325, 550, 1000, 50000};
constexpr int pieceColor[13] = {Side::Both,
    Side::White, Side::White, Side::White, Side::White, Side::White, Side::White,
    Side::Black, Side::Black, Side::Black, Side::Black, Side::Black, Side::Black};

constexpr int piecePawn[13] = {false, true, false, false, false, false, false, true, false, false, false, false, false};
constexpr int pieceKnight[13] = {false, false, true, false, false, false, false, false, true, false, false, false, false};
constexpr int pieceKing[13] = {false, false, false, false, false, false, true, false, false, false, false, false, true};
constexpr int pieceRookQueen[13] = {false, false, false, false, true, true, false, false, false, false, true, true, false};
constexpr int pieceBishopQueen[13] = {false, false, false, true, false, true, false, false, false, true, false, true, false};

constexpr int knDir[8] = { -8, -19,	-21, -12, 8, 19, 21, 12 };
constexpr int rkDir[4] = { -1, -10,	1, 10 };
constexpr int biDir[4] = { -9, -11, 11, 9 };
constexpr int kiDir[8] = { -1, -10,	1, 10, -9, -11, 11, 9 };

constexpr int castleHList[120] = {
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

constexpr int mirror[64] = {
    56	,	57	,	58	,	59	,	60	,	61	,	62	,	63	,
    48	,	49	,	50	,	51	,	52	,	53	,	54	,	55	,
    40	,	41	,	42	,	43	,	44	,	45	,	46	,	47  ,
    32	,	33	,	34	,	35	,	36	,	37	,	38	,	39	,
    24	,	25	,	26	,	27	,	28	,	29	,	30	,	31	,
    16	,	17	,	18	,	19	,	20	,	21	,	22	,	23	,
    8	,	9	,	10	,	11	,	12	,	13	,	14	,	15	,
    0	,	1	,	2	,	3	,	4	,	5	,	6	,	7
};

constexpr int EndgameMatch = (1 * pieceVal[wR] + 2 * pieceVal[wN] + 2 * pieceVal[wP]);

constexpr std::array<int, 64> kingEndgameTable = {
    -50	,	-10	,	0	,	0	,	0	,	0	,	-10	,	-50	,
    -10,	0	,	10	,	10	,	10	,	10	,	0	,	-10	,
    0	,	10	,	20	,	20	,	20	,	20	,	10	,	0	,
    0	,	10	,	20	,	40	,	40	,	20	,	10	,	0	,
    0	,	10	,	20	,	40	,	40	,	20	,	10	,	0	,
    0	,	10	,	20	,	20	,	20	,	20	,	10	,	0	,
    -10,	0	,	10	,	10	,	10	,	10	,	0	,	-10	,
    -50	,	-10	,	0	,	0	,	0	,	0	,	-10	,	-50
};

constexpr std::array<int, 64> kingOpeningTable = {
    0	,	5	,	5	,	-10	,	-10	,	0	,	10	,	5	,
    -30	,	-30	,	-30	,	-30	,	-30	,	-30	,	-30	,	-30	,
    -50	,	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,
    -70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
    -70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
    -70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
    -70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
    -70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70
};

constexpr std::array<int, 64> pawnScoreTable = {
    0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
    10	,	10	,	0	,	-10	,	-10	,	0	,	10	,	10	,
    5	,	0	,	0	,	5	,	5	,	0	,	0	,	5	,
    0	,	0	,	10	,	20	,	20	,	10	,	0	,	0	,
    5	,	5	,	5	,	10	,	10	,	5	,	5	,	5	,
    10	,	10	,	10	,	20	,	20	,	10	,	10	,	10	,
    20	,	20	,	20	,	30	,	30	,	20	,	20	,	20	,
    0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

constexpr std::array<int, 64> knightScoreTable = {
    0	,	-10	,	0	,	0	,	0	,	0	,	-10	,	0	,
    0	,	0	,	0	,	5	,	5	,	0	,	0	,	0	,
    0	,	0	,	10	,	10	,	10	,	10	,	0	,	0	,
    0	,	0	,	10	,	20	,	20	,	10	,	5	,	0	,
    5	,	10	,	15	,	20	,	20	,	15	,	10	,	5	,
    5	,	10	,	10	,	20	,	20	,	10	,	10	,	5	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

constexpr std::array<int, 64> bishopScoreTable = {
    0	,	0	,	-10	,	0	,	0	,	-10	,	0	,	0	,
    0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
    0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
    0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
    0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
    0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
    0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
    0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

constexpr std::array<int, 64> rookScoreTable = {
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    25	,	25	,	25	,	25	,	25	,	25	,	25	,	25	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0
};

constexpr std::array<int, 64> queenScoreTable = {
    0	,	0	,	-5	,	10	,	10	,	0	,	0	,	-5	,
    0	,	0	,	0	,	20	,	20	,	0	,	0	,	0	,
    0	,	0	,	15	,	25	,	25	,	0	,	0	,	15	,
    0	,	10	,	20	,	35	,	35	,	0	,	10	,	20	,
    0	,	10	,	20	,	40	,	40	,	0	,	10	,	20	,
    0	,	0	,	15	,	30	,	30	,	0	,	0	,	15	,
    25	,	25	,	25	,	25	,	25	,	25	,	25	,	25	,
    0	,	0	,	5	,	10	,	10	,	0	,	0	,	5	,
};


constexpr std::array<int, 64> blackSq = {
    56	,	57	,	58	,	59	,	60	,	61	,	62	,	63	,
    48	,	49	,	50	,	51	,	52	,	53	,	54	,	55	,
    40	,	41	,	42	,	43	,	44	,	45	,	46	,	47  ,
    32	,	33	,	34	,	35	,	36	,	37	,	38	,	39	,
    24	,	25	,	26	,	27	,	28	,	29	,	30	,	31	,
    16	,	17	,	18	,	19	,	20	,	21	,	22	,	23	,
    8	,	9	,	10	,	11	,	12	,	13	,	14	,	15	,
    0	,	1	,	2	,	3	,	4	,	5	,	6	,	7
};

constexpr int isolatedPawn = -10;
constexpr int bishopPairScore = 30;
constexpr int passedPawn[] = {0, 5, 10, 20, 35, 60, 100, 250};
constexpr int rookOpenFile = 50;
constexpr int rookSemiOpenFile = 5;
constexpr int queenOpenFile = 5;
constexpr int queenSemiOpenFile = 3;
constexpr int pawnIslandScore = -2;