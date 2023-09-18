//
// Created by moe on 1/23/21.
//

#include "../include/board.h"
#include <iostream>
#include <functional>
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

std::string Board::squareToText(Squares sq) const {
    char file = 'a' + filesBoard.at(sq);
    char rank = '1' + ranksBoard.at(sq);

    std::string ret = std::string() + file + rank;
    return ret;
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

bool Board::loadFEN(std::string fenStr) {

    int rank = Ranks::Eight;
    int file = Files::A;
    Pieces piece = Pieces::EMPTY;
    int count;
    Squares sq64, sq120;

    this->reset();

    std::string::iterator fen = std::begin(fenStr);

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
                      std::cout << "Invalid FEN format.\n";
                      return false;
        }
        for(int i = 0; i < count; i++) {
            sq64 = static_cast<Squares>(rank * 8 + file);
            sq120 = static_cast<Squares>(getSquareIndex(SQ::SQ120, sq64));
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

std::uint64_t Board::generateHashKey() const {
    std::uint64_t key = 0ull;
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

const bool Board::isSquareAttackedOptm(Squares sq, Side attackingSide) const {
    Pieces tPiece;
    int tSq;

    //Pawns Attack;
    if(attackingSide == Side::White) {
        if (pieces.at(sq - 11) == Pieces::wP || pieces.at(sq - 9) == Pieces::wP) {
            return true;
        }
    } else {
        if (pieces.at(sq + 11) == Pieces::bP || pieces.at(sq + 9) == Pieces::bP) {
            return true;
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
                    return true;
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
                    return true;
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
                    return true;
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
                return true;
            }
            break;
        }
    }
    return false;
}

const std::tuple<bool, int> Board::isSquareAttacked(Squares sq, Side attackingSide) const {
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

    //removing the piece from the pieceList
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

    hashSide();

    if(isSquareAttackedOptm(kingSq.at(getOppSide()), side)) {
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

inline void Board::hashCastling() {
    posKey ^= castleHK[castlePerm];
}

bool Board::check() const {

    int t_pceNum[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int t_bigPce[2] = { 0, 0};
    int t_majPce[2] = { 0, 0};
    int t_minPce[2] = { 0, 0};
    int t_material[2] = { 0, 0};

    int sq120,colour;

    BitBoard t_pawns[3];

    t_pawns[Side::White] = pawns.at(Side::White);
    t_pawns[Side::Black] = pawns[Side::Black];
    t_pawns[Side::Both] = pawns[Side::Both];

    for(Pieces tPiece = Pieces::wP; tPiece <= Pieces::bK; ++tPiece) {
        for(int tPceNum = 0; tPceNum < piecesNum.at(tPiece); ++tPceNum) {
            sq120 = getFromPiecesList((Pieces)tPiece, tPceNum);
        }
    }

    for(int sq64 = 0; sq64 < 64; ++sq64) {
        sq120 = getSquareIndex(SQ::SQ120, sq64);
        Pieces tPiece = pieces.at(sq120);
        t_pceNum[tPiece]++;
        colour = pieceColor[tPiece];

        if( pieceBig[tPiece] == true) t_bigPce[colour]++;
        if( pieceMin[tPiece] == true) t_minPce[colour]++;
        if( pieceMaj[tPiece] == true) t_majPce[colour]++;

        t_material[colour] += pieceVal[tPiece];
    }

    for(Pieces tPiece = Pieces::wP; tPiece <= Pieces::bK; ++tPiece) {
        assert(t_pceNum[tPiece] == this->piecesNum[tPiece]);
    }

    assert(t_pawns[Side::White].count() == pawns.at(Side::White).count());

    assert(t_pawns[Side::Black].count() == pawns.at(Side::Black).count());

    assert(t_pawns[Side::Both].count() == pawns.at(Side::Both).count());

    while(t_pawns[Side::White].count()) {
        assert(pieces[getSquareIndex(SQ::SQ120, t_pawns[Side::White].pop())] == Pieces::wP);
    }

    while(t_pawns[Side::Black].count()) {
        assert(pieces[getSquareIndex(SQ::SQ120, t_pawns[Side::Black].pop())] == Pieces::bP);
    }

    while(t_pawns[Side::Both].count()) {
        int sq64 = t_pawns[Side::Both].pop();
        assert(pieces[getSquareIndex(SQ::SQ120, sq64)] == Pieces::wP || pieces[getSquareIndex(SQ::SQ120, sq64)] == Pieces::bP);
    }

    assert(t_material[Side::White] == material[Side::White] && t_material[Side::Black] == material[Side::Black]);
    assert(t_minPce[Side::White] == minPieces[Side::White] && t_minPce[Side::Black] == minPieces[Side::Black]);
    assert(t_majPce[Side::White] == majPieces[Side::White] && t_majPce[Side::Black] == majPieces[Side::Black]);
    assert(t_bigPce[Side::White] == bigPieces[Side::White] && t_bigPce[Side::Black] == bigPieces[Side::Black]);

    assert(side == Side::White || side == Side::Black);

    assert(pieces[this->kingSq[Side::White]] == Pieces::wK);
    assert(pieces[this->kingSq[Side::Black]] == Pieces::bK);

    return true;
}

bool Board::isRep() const {

    for(int i = hisPly - fiftyMoves; i < hisPly-1; i++) {
        if(posKey == history.at(i).posKey)
            return true;
    }
    return false;
}

bool Board::materialDraw() const {
    if (!piecesNum.at(Pieces::wR) && !piecesNum.at(Pieces::bR) && !piecesNum.at(Pieces::wQ) && !piecesNum.at(Pieces::bQ)) {
        if (!piecesNum.at(bB) && !piecesNum.at(wB)) {
            if (piecesNum.at(Pieces::wN) < 3 && piecesNum.at(Pieces::bN) < 3) {  return true; }
        } else if (!piecesNum.at(Pieces::wN) && !piecesNum.at(Pieces::bN)) {
            if (abs(piecesNum.at(Pieces::wB) - piecesNum.at(Pieces::bB)) < 2) { return true; }
        } else if ((piecesNum.at(Pieces::wN) < 3 && !piecesNum.at(Pieces::wB)) || (piecesNum.at(Pieces::wB) == 1 && !piecesNum.at(Pieces::wN))) {
            if ((piecesNum.at(Pieces::bN) < 3 && !piecesNum.at(Pieces::bB)) || (piecesNum.at(Pieces::bB) == 1 && !piecesNum.at(Pieces::bN)))  { return true; }
        }
    } else if (!piecesNum.at(Pieces::wQ) && !piecesNum.at(Pieces::bQ)) {
        if (piecesNum.at(Pieces::wR) == 1 && piecesNum.at(Pieces::bR) == 1) {
            if ((piecesNum.at(Pieces::wN) + piecesNum.at(Pieces::wB)) < 2 && (piecesNum.at(Pieces::bN) + piecesNum.at(Pieces::bB)) < 2)	{ return true; }
        } else if (piecesNum.at(wR) == 1 && !piecesNum.at(bR)) {
            if ((piecesNum.at(Pieces::wN) + piecesNum.at(Pieces::wB) == 0) && (((piecesNum.at(Pieces::bN) + piecesNum.at(Pieces::bB)) == 1) || ((piecesNum.at(Pieces::bN) + piecesNum.at(Pieces::bB)) == 2))) { return true; }
        } else if (piecesNum.at(Pieces::bR) == 1 && !piecesNum.at(Pieces::wR)) {
            if ((piecesNum.at(Pieces::bN) + piecesNum.at(Pieces::bB) == 0) && (((piecesNum.at(Pieces::wN) + piecesNum.at(Pieces::wB)) == 1) || ((piecesNum.at(Pieces::wN) + piecesNum.at(Pieces::wB)) == 2))) { return true; }
        }
    }

    return false;
}

int Board::eval() const {
    int score = material.at(Side::White) - material.at(Side::Black);
    int sq;

    if(!piecesNum.at(Pieces::wP) && !piecesNum.at(Pieces::bP))
        if(materialDraw()) return 0;

    for(int i = 0; i < piecesNum.at(Pieces::wP); i++) {
        sq = getFromPiecesList(Pieces::wP, i);
        score += pawnScoreTable[getSquareIndex(SQ::SQ64, sq)];

        if( (isolatedPawnsMask[(int)getSquareIndex(SQ::SQ64, sq)] & pawns.at(Side::White).getBitmask() ) == 0 ) {
            score += isolatedPawn;
        }

        if( (whitePassersMask[(int)getSquareIndex(SQ::SQ64, sq)] & pawns.at(Side::Black).getBitmask() ) == 0 ) {
            score += passedPawn[ranksBoard.at(sq)];
        }
    }

    for(int i = 0; i < piecesNum.at(Pieces::bP); i++) {
        sq = getFromPiecesList(Pieces::bP, i);
        score -= pawnScoreTable[GETBLACKSQ(getSquareIndex(SQ::SQ64, sq))];

        if( (isolatedPawnsMask[getSquareIndex(SQ::SQ64, sq)] & pawns.at(Side::Black).getBitmask() ) == 0 ) {
            score -= isolatedPawn;
        }

        if( (blackPassersMask[getSquareIndex(SQ::SQ64, sq)] & pawns.at(Side::White).getBitmask() ) == 0 ) {
            score -= passedPawn[7 - ranksBoard.at(sq)];
        }
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

        if(! (pawns.at(Side::Both).getBitmask() & fileBBMask[filesBoard.at(sq)]) )
            score+= rookOpenFile;
        else if(! (pawns.at(Side::White).getBitmask() & fileBBMask[filesBoard.at(sq)]) )
            score+= rookSemiOpenFile;
    }

    for(int i = 0; i < piecesNum.at(Pieces::bR); i++) {
        sq = getFromPiecesList(Pieces::bR, i);
        score -= rookScoreTable[GETBLACKSQ(getSquareIndex(SQ::SQ64, sq))];

        if(! (pawns.at(Side::Both).getBitmask() & fileBBMask[filesBoard.at(sq)]) )
            score-= rookOpenFile;
        else if(! (pawns.at(Side::Black).getBitmask() & fileBBMask[filesBoard.at(sq)]) )
            score-= rookSemiOpenFile;
    }

    for(int i = 0; i < piecesNum.at(Pieces::wQ); i++) {
        sq = getFromPiecesList(Pieces::wQ, i);
        score += queenScoreTable[getSquareIndex(SQ::SQ64, sq)];

        if(! (pawns.at(Side::Both).getBitmask() & fileBBMask[filesBoard.at(sq)]) )
            score+= queenOpenFile;
        else if(! (pawns.at(Side::White).getBitmask() & fileBBMask[filesBoard.at(sq)]) )
            score+= queenSemiOpenFile;
    }

    for(int i = 0; i < piecesNum.at(Pieces::bQ); i++) {
        sq = getFromPiecesList(Pieces::bQ, i);
        score -= queenScoreTable[GETBLACKSQ(getSquareIndex(SQ::SQ64, sq))];

        if(! (pawns.at(Side::Both).getBitmask() & fileBBMask[filesBoard.at(sq)]) )
            score-= queenOpenFile;
        else if(! (pawns.at(Side::Black).getBitmask() & fileBBMask[filesBoard.at(sq)]) )
            score-= queenSemiOpenFile;
    }

    sq = getFromPiecesList(Pieces::wK, 0);
    if(material.at(Side::Black) - pieceVal[wK] < EndgameMatch) {
        score += kingEndgameTable.at(getSquareIndex(SQ::SQ64, sq));
    }
    else
        score += kingOpeningTable.at(getSquareIndex(SQ::SQ64, sq));

    sq = getFromPiecesList(Pieces::bK, 0);
    if(material.at(Side::White) - pieceVal[wK] < EndgameMatch)
        score -= kingEndgameTable.at(mirrorSquare64((Squares)getSquareIndex(SQ::SQ64, sq)));
    else
        score -= kingOpeningTable.at(mirrorSquare64((Squares)getSquareIndex(SQ::SQ64, sq)));

    if(piecesNum.at(Pieces::wB) >= 2) score += bishopPairScore;
    if(piecesNum.at(Pieces::bB) >= 2) score -= bishopPairScore;

    //pawns islands

    constexpr uint64_t FILE_A = 0x0101010101010101ULL;
    constexpr uint64_t FILE_B = FILE_A << 1;
    constexpr uint64_t FILE_C = FILE_B << 1;
    constexpr uint64_t FILE_D = FILE_C << 1;
    constexpr uint64_t FILE_E = FILE_D << 1;
    constexpr uint64_t FILE_F = FILE_E << 1;
    constexpr uint64_t FILE_G = FILE_F << 1;
    constexpr uint64_t FILE_H = FILE_G << 1;

    int whitePawnIslands = 0;
    int blackPawnIslands = 0;

    uint64_t whitePawns = pawns.at(Side::White).getBitmask();
    uint64_t blackPawns = pawns.at(Side::Black).getBitmask();

    const uint64_t FILE_AB = FILE_A | FILE_B;
    const uint64_t FILE_GH = FILE_G | FILE_H;

    uint64_t temp;
    while (whitePawns != 0) {
        uint64_t lsb = whitePawns & -whitePawns;
        whitePawns ^= lsb;

        temp = (lsb >> 1) & ~FILE_H;
        if ((temp & whitePawns) == 0 && (temp & blackPawns) == 0) {
            temp = (lsb << 1) & ~FILE_A;
            if ((temp & whitePawns) == 0 && (temp & blackPawns) == 0) {
                whitePawnIslands++;
            }
        }
    }

    while (blackPawns != 0) {
        uint64_t lsb = blackPawns & -blackPawns;
        blackPawns ^= lsb;

        temp = (lsb << 1) & ~FILE_A;
        if ((temp & whitePawns) == 0 && (temp & blackPawns) == 0) {
            temp = (lsb >> 1) & ~FILE_H;
            if ((temp & whitePawns) == 0 && (temp & blackPawns) == 0) {
                blackPawnIslands++;
            }
        }
    }

    if(whitePawnIslands > 1) {
        if(piecesNum.at(Pieces::wQ))
            score += whitePawnIslands * pawnIslandScore;
        else
            score += 2 * (whitePawnIslands * pawnIslandScore);
    }

    if(blackPawnIslands > 1) {
        if(piecesNum.at(Pieces::bQ))
            score -= blackPawnIslands * pawnIslandScore;
        else
            score -= 2 * (blackPawnIslands * pawnIslandScore);
    }

    if(side == Side::White)
        return score;
    else
        return -score;
}

std::uint64_t Board::getKey() const {
    return posKey;
}

Side Board::getOppSide() {
    if(this->side == Side::White)
        return Side::Black;
    return Side::White;
}

void Board::mirror() {
    Board temp;
    temp.enPas = Squares::NO_SQ;
    Pieces tPiece;
    Side tSide = getOppSide();
    int swappedPieces[] = {EMPTY, bP, bN, bB, bR, bQ, bK, wP, wN, wB, wR, wQ, wK};

    if(this->castlePerm & Castling::WK) temp.castlePerm |= Castling::WK;
    if(this->castlePerm & Castling::WQ) temp.castlePerm |= Castling::WQ;

    if(this->castlePerm & Castling::BK) temp.castlePerm |= Castling::BK;
    if(this->castlePerm & Castling::BQ) temp.castlePerm |= Castling::BQ;

    if(this->enPas != Squares::NO_SQ)
        temp.enPas = (Squares)getSquareIndex(SQ::SQ120, mirrorSquare64((Squares)getSquareIndex(SQ::SQ64, this->enPas)));

    for(int i = 0; i < 64; i++)
        temp.pieces.at(i) = this->pieces.at(getSquareIndex(SQ::SQ120, mirrorSquare64((Squares)i)));

    this->reset();

    for(int i = 0; i < 64; i++) {
        tPiece = (Pieces) swappedPieces[temp.pieces[i]];
        this->pieces.at(getSquareIndex(SQ::SQ120, i)) = tPiece;
    }

    this->castlePerm = temp.castlePerm;
    this->enPas = temp.enPas;
    this->side = tSide;
    this->posKey = generateHashKey();

    updateMaterials();
}

void Board::makeNullMove() {
    ply++;
    history.at(hisPly).posKey = posKey;

    if(enPas != Squares::NO_SQ) hashEnPassant();

    history.at(hisPly).move = InvalidMove();
    history.at(hisPly).fiftyMoves = fiftyMoves;
    history.at(hisPly).enPas = enPas;
    history.at(hisPly).castlePerm = castlePerm;
    enPas = Squares::NO_SQ;

    side = getOppSide();
    hisPly++;
    hashSide();
}

void Board::undoNullMove() {
    hisPly--;
    ply--;

    if(enPas != Squares::NO_SQ) hashEnPassant();

    fiftyMoves = history.at(hisPly).fiftyMoves;
    enPas = history.at(hisPly).enPas;
    castlePerm =  history.at(hisPly).castlePerm;

    if(enPas != Squares::NO_SQ) hashEnPassant();

    side = getOppSide();
    hashSide();
}
