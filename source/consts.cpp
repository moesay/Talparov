#include "../include/consts.h"

char pieceChar[] = ".PNBRQKpnbrqk";
char sideChar[] = "wb-";
char rankChar[] = "12345678";
char fileChar[] = "abcdefgh";

int pieceBig[13] = {false, false, true, true, true, true, true, false, true, true, true, true, true};
int pieceMaj[13] = {false, false, false, false, true, true, true, false, false, false, true, true, true};
int pieceMin[13] = {false, false, true, true, false, false, false, false, true, true, false, false, false};
int pieceVal[13] = {0, 100, 325, 325, 550, 1000, 50000, 100, 325, 325, 550, 1000, 50000};
int pieceColor[13] = {Side::Both,
                      Side::White, Side::White, Side::White, Side::White, Side::White, Side::White,
                      Side::Black, Side::Black, Side::Black, Side::Black, Side::Black, Side::Black};

int pieceKnight[13] = {false, false, true, false, false, false, false, false, true, false, false, false, false};
int pieceKing[13] = {false, false, false, false, false, false, true, false, false, false, false, false, true};
int pieceRookQueen[13] = {false, false, false, false, true, true, false, false, false, false, true, true, false};
int pieceBishopQueen[13] = {false, false, false, true, false, true, false, false, false, true, false, true, false};
