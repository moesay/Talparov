//
// Created by moe on 1/23/21.
//

#include "../include/board.h"
#include <iostream>
#include <functional>

static constexpr std::array<int, 64> pawnScoreTable = {
    0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
    10	,	10	,	0	,	-10	,	-10	,	0	,	10	,	10	,
    5	,	0	,	0	,	5	,	5	,	0	,	0	,	5	,
    0	,	0	,	10	,	20	,	20	,	10	,	0	,	0	,
    5	,	5	,	5	,	10	,	10	,	5	,	5	,	5	,
    10	,	10	,	10	,	20	,	20	,	10	,	10	,	10	,
    20	,	20	,	20	,	30	,	30	,	20	,	20	,	20	,
    0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

static constexpr std::array<int, 64> knightScoreTable = {
    0	,	-10	,	0	,	0	,	0	,	0	,	-10	,	0	,
    0	,	0	,	0	,	5	,	5	,	0	,	0	,	0	,
    0	,	0	,	10	,	10	,	10	,	10	,	0	,	0	,
    0	,	0	,	10	,	20	,	20	,	10	,	5	,	0	,
    5	,	10	,	15	,	20	,	20	,	15	,	10	,	5	,
    5	,	10	,	10	,	20	,	20	,	10	,	10	,	5	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

static constexpr std::array<int, 64> bishopScoreTable = {
    0	,	0	,	-10	,	0	,	0	,	-10	,	0	,	0	,
    0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
    0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
    0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
    0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
    0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
    0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
    0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

static constexpr std::array<int, 64> rookScoreTable = {
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    25	,	25	,	25	,	25	,	25	,	25	,	25	,	25	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0
};

static constexpr std::array<int, 64> queenScoreTable = {
    0	,	0	,	-5	,	10	,	10	,	0	,	0	,	-5	,
    0	,	0	,	0	,	20	,	20	,	0	,	0	,	0	,
    0	,	0	,	15	,	25	,	25	,	0	,	0	,	15	,
    0	,	10	,	20	,	35	,	35	,	0	,	10	,	20	,
    0	,	10	,	20	,	40	,	40	,	0	,	10	,	20	,
    0	,	0	,	15	,	30	,	30	,	0	,	0	,	15	,
    25	,	25	,	25	,	25	,	25	,	25	,	25	,	25	,
    0	,	0	,	5	,	10	,	10	,	0	,	0	,	5	,
};


static constexpr std::array<int, 64> blackSq = {
    56	,	57	,	58	,	59	,	60	,	61	,	62	,	63	,
    48	,	49	,	50	,	51	,	52	,	53	,	54	,	55	,
    40	,	41	,	42	,	43	,	44	,	45	,	46	,	47  ,
    32	,	33	,	34	,	35	,	36	,	37	,	38	,	39	,
    24	,	25	,	26	,	27	,	28	,	29	,	30	,	31	,
    16	,	17	,	18	,	19	,	20	,	21	,	22	,	23	,
    8	,	9	,	10	,	11	,	12	,	13	,	14	,	15	,
    0	,	1	,	2	,	3	,	4	,	5	,	6	,	7
};


#define GETBLACKSQ(sq) (blackSq[(sq)])

void Board::reset() {
    for(auto &sq : pieces)
        sq = (Pieces)Squares::OFF_BOARD;

    for(int i = 0; i < 64; i++)
        pieces.at(getSquareIndex(SQ::SQ120, i)) = Pieces::EMPTY;

    for(int i = 0; i < 2; i++) {
        bigPieces.at(i) = 0;
        majPieces.at(i) = 0;
        minPieces.at(i) = 0;
        material.at(i) = 0;
    }

    for(auto &pawn : pawns)
        pawn = 0ull;

    for(auto &i : piecesNum)
        i = 0;

    kingSq[Side::White] = kingSq[Side::Black] = Squares::NO_SQ;

    side = Side::Both;
    enPas = Squares::NO_SQ;
    fiftyMoves = 0;
    ply = 0;
    hisPly = 0;

    castlePerm = 0;
    posKey = 0;
}

void Board::print() const {
    Squares sq;
    Pieces piece;
    for (int rank = Ranks::Eight; rank >= Ranks::One; rank--) {
        std::cout << rank + 1 << "  ";
        for (int file = Files::A; file <= Files::H; file++) {
            sq = FRtoSq((Files) file, (Ranks) rank);
            piece = pieces.at((int) sq);
            std::cout << pieceChar[piece] << "  ";
        }
        std::cout << "\n";
    }

    std::cout << "\n";

    for (int file = Files::A; file <= Files::H; file++) {
        std::cout << "  " << (char) ('a' + file);
    }

    std::cout << "\n";

    std::cout << "Side : " << (side == Side::White ? "White\n" : "Black\n");

    if (enPas != Squares::NO_SQ) {
        char enPasFile = 'a' + filesBoard.at(enPas);
        int enPasRank = 1 + ranksBoard.at(enPas);
        std::cout << "en Passant : " << enPasFile << enPasRank << "\n";
    }
    else {
        std::cout << "en Passant : " << "NO_SQ\n";
    }
    std::cout << "Castling : "
        << (castlePerm & Castling::WK ? "K" : "-")
        << (castlePerm & Castling::WQ ? "Q" : "-")
        << (castlePerm & Castling::BK ? "k" : "-")
        << (castlePerm & Castling::BQ ? "q" : "-") << "\n";

    std::cout << "Position HashKey : " << std::hex << posKey << "\n";

}

void Board::updateMaterials() {
    Squares sq;
    int color;
    Pieces piece;
    for(int i = 0; i < SQUARES; i++) {
        sq = (Squares)i;
        piece = pieces.at(i);

        if(piece != (Pieces)Squares::OFF_BOARD && piece != Pieces::EMPTY) {
            color = pieceColor[piece];
            if(pieceBig[piece] == true) bigPieces.at(color)++;
            if(pieceMin[piece] == true) minPieces.at(color)++;
            if(pieceMaj[piece] == true) majPieces.at(color)++;

            material.at(color) += pieceVal[piece];
            setPiecesList(piece, piecesNum.at(piece)) = sq;
            piecesNum.at(piece)++;

            if(piece == Pieces::wK) kingSq.at(Side::White) = sq;
            if(piece == Pieces::bK) kingSq.at(Side::Black) = sq;

            if(piece == Pieces::wP) {
                pawns.at(Side::White).set(sq);
                pawns.at(Side::Both).set(sq);
            } else if (piece == Pieces::bP) {
                pawns.at(Side::Black).set(sq);
                pawns.at(Side::Both).set(sq);
            }
        }
    }
}

bool Board::loadFEN(const char* fen) {

    int rank = Ranks::Eight;
    int file = Files::A;
    Pieces piece = Pieces::EMPTY;
    int count;
    int sq64, sq120;

    this->reset();

    while((rank >= Ranks::One) && *fen) {
        count = 1;
        switch (*fen) {
            case 'p': piece = Pieces::bP; break;
            case 'r': piece = Pieces::bR; break;
            case 'n': piece = Pieces::bN; break;
            case 'b': piece = Pieces::bB; break;
            case 'k': piece = Pieces::bK; break;
            case 'q': piece = Pieces::bQ; break;
            case 'P': piece = Pieces::wP; break;
            case 'R': piece = Pieces::wR; break;
            case 'N': piece = Pieces::wN; break;
            case 'B': piece = Pieces::wB; break;
            case 'K': piece = Pieces::wK; break;
            case 'Q': piece = Pieces::wQ; break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                      piece = Pieces::EMPTY;
                      count = *fen - '0';
                      break;

            case '/':
            case ' ':
                      rank--;
                      file = Files::A;
                      fen++;
                      continue;

            default:
                      std::cout << "FEN format error.\n";
                      return false;
        }
        for(int i = 0; i < count; i++) {
            sq64 = rank * 8 + file;
            sq120 = getSquareIndex(SQ::SQ120, sq64);
            if(piece != Pieces::EMPTY)
                pieces.at(sq120) = piece;
            file++;
        }
        fen++;
    }
    assert(*fen == 'w' || *fen == 'b');
    side = (*fen == 'w' ? Side::White : Side::Black);
    fen += 2;

    for(int i = 0; i < 4; i++) {
        if(*fen == ' ') break;
        switch (*fen) {
            case 'K' : castlePerm |= Castling::WK; break;
            case 'Q' : castlePerm |= Castling::WQ; break;
            case 'k' : castlePerm |= Castling::BK; break;
            case 'q' : castlePerm |= Castling::BQ; break;
            default : break;
        }
        fen++;
    }
    fen++;
    if(*fen != '-') {
        file = fen[0] - 'a';
        rank = fen[1] - '1';
        enPas = FRtoSq((Files)file, (Ranks)rank);
    }

    posKey = this->generateHashKey();
    this->updateMaterials();
    return true;
}

u64 Board::generateHashKey() const {
    u64 key = 0ull;
    int sq = 0;
    for(auto piece : pieces) {
        if(piece != Squares::NO_SQ && piece != Pieces::EMPTY && piece != Squares::OFF_BOARD) {
            key ^= piecesHK[piece][sq];
            sq++;
        }
    }
    if(side == Side::White)
        key ^= sideHK;
    if(enPas != Squares::NO_SQ)
        key ^= piecesHK[Pieces::EMPTY][enPas];
    key ^= castleHK[castlePerm];
    return key;
}

const std::tuple<bool, int> Board::isSquareAttacked(Squares sq, Side attackingSide) const {
    constexpr int knDir[8] = { -8, -19,	-21, -12, 8, 19, 21, 12 };
    constexpr int rkDir[4] = { -1, -10,	1, 10 };
    constexpr int biDir[4] = { -9, -11, 11, 9 };
    constexpr int kiDir[8] = { -1, -10,	1, 10, -9, -11, 11, 9 };

    bool attacked = false;
    int noOfAttackers = 0;

    Pieces tPiece;
    int tSq;

    //Pawns Attack;
    if(attackingSide == Side::White) {
        if (pieces.at(sq - 11) == Pieces::wP || pieces.at(sq - 9) == Pieces::wP) {
            attacked = true;
            noOfAttackers++;
        }
    } else {
        if (pieces.at(sq + 11) == Pieces::bP || pieces.at(sq + 9) == Pieces::bP) {
            attacked = true;
            noOfAttackers++;
        }
    }

    //Knights Attack
    for(auto dir : knDir) {
        tSq = sq + dir;
        tPiece = pieces.at(tSq);
        while (tPiece != Squares::OFF_BOARD) {
            if(tPiece != Pieces::EMPTY) {
                if(isItKnight(tPiece)
                        && pieceColor[tPiece] == attackingSide) {
                    attacked = true;
                    noOfAttackers++;
                }
            }
            break;
        }
    }

    //Rooks and Queens Attack
    for(auto dir : rkDir) {
        tSq = sq + dir;
        tPiece = pieces.at(tSq);
        while (tPiece != Squares::OFF_BOARD) {
            if(tPiece != Pieces::EMPTY) {
                if(isItRookOrQueen(tPiece) &&
                        pieceColor[tPiece] == attackingSide) {
                    attacked = true;
                    noOfAttackers++;
                }
                break;
            }
            tSq += dir;
            tPiece = pieces.at(tSq);
        }
    }

    //Bishops and Queens Attack
    for(auto dir : biDir) {
        tSq = sq + dir;
        tPiece = pieces.at(tSq);
        while (tPiece != Squares::OFF_BOARD) {
            if (tPiece != Pieces::EMPTY) {
                if (isItBishopOrQueen(tPiece) &&
                        pieceColor[tPiece] == attackingSide) {
                    attacked = true;
                    noOfAttackers++;
                }
                break;
            }
            tSq += dir;
            tPiece = pieces.at(tSq);
        }
    }

    //King Attack
    for(auto dir : kiDir) {
        tSq = sq + dir;
        tPiece = pieces.at(tSq);
        while (tPiece != Squares::OFF_BOARD) {
            if(isItKing(tPiece)
                    && pieceColor[tPiece] == attackingSide) {
                attacked = true;
                noOfAttackers++;
            }
            break;
        }
    }

    return std::make_tuple(attacked, noOfAttackers);
}

void Board::allAttackedSquares(Side attackingSide) const {
    for(int rank = Ranks::Eight; rank >= Ranks::One; rank--) {
        std::cout << rank+1 << " ";
        for(int file = Files::A; file <= Files::H; file++) {
            auto [isAttacked, noOfAttackers] = isSquareAttacked(FRtoSq((Files)file, (Ranks)rank), attackingSide);

            std::cout << " " << noOfAttackers << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n  ";
    for(int file = Files::A; file <= Files::H; file++)
        std::cout << " " << static_cast<char>('a'+file) << " ";
}

inline void Board::hashPiece(Pieces piece, Squares square) {
    posKey ^= piecesHK[piece][square];
}

inline void Board::hashSide() {
    posKey ^= sideHK;
}

inline void Board::hashEnPassant() {
    posKey ^= piecesHK[Pieces::EMPTY][enPas];
}

void Board::clearPiece(Squares square) {
    Pieces piece = pieces.at(square);


    int color = pieceColor[piece];
    int tPiece = 0;
    hashPiece(piece, square);

    pieces.at(square) = Pieces::EMPTY;
    material.at(color) -= pieceVal[piece];

    if(pieceBig[piece]) {
        bigPieces.at(color)--;
        if(pieceMaj[piece]) {
            majPieces.at(color)--;
        }
        else {
            minPieces.at(color)--;
        }
    }
    else {
        pawns.at(color).unset(square);
        pawns.at(Side::Both).unset(square);
    }

    for(int i = 0; i < piecesNum.at(piece); i++) {
        if(getFromPiecesList(piece, i) == square) {
            tPiece = i;
            break;
        }
    }

    piecesNum.at(piece)--;
    setPiecesList(piece, tPiece) = getFromPiecesList(piece, piecesNum.at(piece));
}

void Board::addPiece(Squares square, Pieces piece) {

    int color = pieceColor[piece];
    hashPiece(piece, square);

    pieces.at(square) = piece;

    if(pieceBig[piece]) {
        bigPieces.at(color)++;
        if(pieceMaj[piece]) {
            majPieces.at(color)++;
        }
        else {
            minPieces.at(color)++;
        }
    }
    else {
        pawns.at(color).set(square);
        pawns.at(Side::Both).set(square);
    }

    material.at(color) += pieceVal[piece];
    setPiecesList(piece, piecesNum.at(piece)++) = square;
}


void Board::movePiece(const Move& move) {
    Pieces piece = pieces.at(move.from);
    int color = pieceColor[piece];

    hashPiece(piece, move.from);
    pieces.at(move.from) = Pieces::EMPTY;

    hashPiece(piece, move.to);
    pieces.at(move.to) = piece;

    if(!pieceBig[piece]) {
        pawns.at(color).unset(move.from);
        pawns.at(Side::Both).unset(move.from);

        pawns.at(color).set(move.to);
        pawns.at(Side::Both).set(move.to);
    }
    for(int i = 0; i < piecesNum.at(piece); i++) {
        if(getFromPiecesList(piece, i) == move.from) {
            setPiecesList(piece, i) = move.to;
            break;
        }
    }
}

bool Board::makeMove(const Move& move) {

    history.at(hisPly).posKey = posKey;
    if(move.enPassant) {
        if(side == Side::White)
            clearPiece((Squares)(move.to - 10));
        else
            clearPiece((Squares)(move.to + 10));
    } else if(move.castling) {
        switch (move.to) {
            case Squares::C1:
                movePiece(Move(Squares::A1, Squares::D1));
                break;
            case Squares::C8:
                movePiece(Move(Squares::A8, Squares::D8));
                break;
            case Squares::G1:
                movePiece(Move(Squares::H1, Squares::F1));
                break;
            case Squares::G8:
                movePiece(Move(Squares::H8, Squares::F8));
                break;
            default: assert(false);
        }
    }

    if(enPas != Squares::NO_SQ)
        hashEnPassant();
    hashCastling();

    history.at(hisPly).move = move;
    history.at(hisPly).fiftyMoves = fiftyMoves;
    history.at(hisPly).enPas = enPas;
    history.at(hisPly).castlePerm = castlePerm;

    castlePerm &= castleHList[move.from];
    castlePerm &= castleHList[move.to];

    enPas = Squares::NO_SQ;

    hashCastling();

    fiftyMoves++;

    if(move.captured != Pieces::EMPTY) {
        fiftyMoves = 0;
        clearPiece(move.to);
    }

    ply++;
    hisPly++;

    if(pieces.at(move.from) == Pieces::wP || pieces.at(move.from) == Pieces::bP) {
        fiftyMoves = 0;
        if(move.pawnStart) {
            if(side == Side::White) {
                enPas = Squares(move.from+10);
            } else {
                enPas = Squares(move.from-10);
            }
            hashEnPassant();
        }
    }
    movePiece(move);

    if(move.promoted != Pieces::EMPTY) {
        clearPiece(move.to);
        addPiece(move.to, move.promoted);
    }

    if(isItKing(pieces.at(move.to))) {
        kingSq.at(side) = move.to;
    }

    if(side == Side::White)
        side = Side::Black;
    else
        side = Side::White;

    Side temp = (side == Side::White ? Side::Black : Side::White);

    hashSide();

    if(isSquareAttacked(kingSq.at(temp), side) != std::make_tuple(false, 0)) {
        undoMove();
        return false;
    }
    return true;
}

void Board::undoMove() {

    hisPly--;
    ply--;
    Move move = history.at(hisPly).move;

    if (enPas != Squares::NO_SQ)
        hashEnPassant();
    hashCastling();

    castlePerm = history.at(hisPly).castlePerm;
    fiftyMoves = history.at(hisPly).fiftyMoves;
    enPas = history.at(hisPly).enPas;

    if (enPas != Squares::NO_SQ)
        hashEnPassant();
    hashCastling();

    side == Side::White ? side = Side::Black : side = Side::White;
    hashSide();

    if (move.enPassant) {
        if (side == Side::White)
            addPiece((Squares) (move.to - 10), Pieces::bP);
        else
            addPiece((Squares) (move.to + 10), Pieces::wP);
    } else if (move.castling) {
        switch (move.to) {
            case Squares::C1:
                movePiece(Move(Squares::D1, Squares::A1));
                break;
            case Squares::C8:
                movePiece(Move(Squares::D8, Squares::A8));
                break;
            case Squares::G1:
                movePiece(Move(Squares::F1, Squares::H1));
                break;
            case Squares::G8:
                movePiece(Move(Squares::F8, Squares::H8));
                break;
            default:
                assert(false);
        }
    }

    Move tMove = move;
    tMove.from = move.to;
    tMove.to = move.from;

    movePiece(tMove);

    if (isItKing(pieces.at(move.from))) {
        kingSq.at(side) = move.from;
    }

    if(move.captured != Pieces::EMPTY) {
        addPiece(move.to, move.captured);
    }

    if(move.promoted != Pieces::EMPTY) {
        assert((move.promoted != Pieces::wP || move.promoted != Pieces::bP) &&
                move.promoted >= Pieces::wP && move.promoted <= Pieces::bK);

        clearPiece(move.from);
        addPiece(move.from, (pieceColor[move.promoted] == Side::White ? Pieces::wP : Pieces::bP));
    }
}

bool Board::isSqOffBoard(const Squares& sq) const {
    return (pieces.at(sq) == Squares::OFF_BOARD);
}

int& Board::setPiecesList(Pieces piece, int index) {
    return pList.at(piece).at(index);
}

int Board::getFromPiecesList(Pieces piece, int index) const {
    return pList.at(piece).at(index);
}

Side Board::getSide() const {
    return side;
}

void Board::hashCastling() {
    posKey ^= castleHK[castlePerm];
}

bool Board::isRep() const {

    for(int i = hisPly - fiftyMoves; i < hisPly-1; i++) {
        if(posKey == history.at(i).posKey)
            return true;
    }
    return false;
}

int Board::eval() const {
    int score = material.at(Side::White) - material.at(Side::Black);
    int sq;

    for(int i = 0; i < piecesNum.at(Pieces::wP); i++) {
        sq = getFromPiecesList(Pieces::wP, i);
        score += pawnScoreTable[getSquareIndex(SQ::SQ64, sq)];
    }

    for(int i = 0; i < piecesNum.at(Pieces::bP); i++) {
        sq = getFromPiecesList(Pieces::bP, i);
        score -= pawnScoreTable[GETBLACKSQ(getSquareIndex(SQ::SQ64, sq))];
    }

    for(int i = 0; i < piecesNum.at(Pieces::wN); i++) {
        sq = getFromPiecesList(Pieces::wN, i);
        score += knightScoreTable[getSquareIndex(SQ::SQ64, sq)];
    }

    for(int i = 0; i < piecesNum.at(Pieces::bN); i++) {
        sq = getFromPiecesList(Pieces::bN, i);
        score -= knightScoreTable[GETBLACKSQ(getSquareIndex(SQ::SQ64, sq))];
    }

    for(int i = 0; i < piecesNum.at(Pieces::wB); i++) {
        sq = getFromPiecesList(Pieces::wB, i);
        score += bishopScoreTable[getSquareIndex(SQ::SQ64, sq)];
    }

    for(int i = 0; i < piecesNum.at(Pieces::bB); i++) {
        sq = getFromPiecesList(Pieces::bB, i);
        score -= bishopScoreTable[GETBLACKSQ(getSquareIndex(SQ::SQ64, sq))];
    }

    for(int i = 0; i < piecesNum.at(Pieces::wR); i++) {
        sq = getFromPiecesList(Pieces::wR, i);
        score += rookScoreTable[getSquareIndex(SQ::SQ64, sq)];
    }

    for(int i = 0; i < piecesNum.at(Pieces::bR); i++) {
        sq = getFromPiecesList(Pieces::bR, i);
        score -= rookScoreTable[GETBLACKSQ(getSquareIndex(SQ::SQ64, sq))];
    }

    for(int i = 0; i < piecesNum.at(Pieces::wQ); i++) {
        sq = getFromPiecesList(Pieces::wQ, i);
        score += queenScoreTable[getSquareIndex(SQ::SQ64, sq)];
    }

    for(int i = 0; i < piecesNum.at(Pieces::bQ); i++) {
        sq = getFromPiecesList(Pieces::bQ, i);
        score -= queenScoreTable[GETBLACKSQ(getSquareIndex(SQ::SQ64, sq))];
    }

    if(side == Side::White)
        return score;
    else
        return -score;
}

u64 Board::getKey() const {
    return posKey;
}
