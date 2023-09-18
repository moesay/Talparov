//
// Created by moe on 1/28/21.
//

#include "../include/movegen.h"
#include <algorithm>
#include <cstdint>
#include <iterator>

int MoveGenerator::mvvLva[13][13];
bool MoveGenerator::set = false;

unsigned long MoveGenerator::leafNodes = 0;

void MoveGenerator::addNonCaptureMove(Move move) {
    if(board->killers[0][board->ply] == move)
        move.score = 900000;
    else if(board->killers[1][board->ply] == move)
        move.score = 800000;
    else
        move.score = board->searchHistory[board->pieces.at(move.from)][move.to];

    moves.emplace_back(move);
}

void MoveGenerator::addCaptureMove(Move move) {
    move.score = mvvLva[move.captured][board->pieces.at(move.from)] + 1000000;
    moves.emplace_back(move);
}

void MoveGenerator::addEnPassantMove(Move move) {
    move.score = 105 + 1000000;
    moves.emplace_back(move);
}

void MoveGenerator::generateAllMoves() {
    moves.clear();
    int sq = 0, tSq = 0;

    if (board->side == Side::White) {
        //loop over every white pawn
        for (int pceNum = 0; pceNum < board->piecesNum.at(Pieces::wP); pceNum++) {
            sq = board->getFromPiecesList(Pieces::wP, pceNum);

            //not capturing moves
            if (board->pieces.at(sq + 10) == Pieces::EMPTY) {
                insertwPMove(Move((Squares) sq, (Squares) (sq + 10)));
                if (ranksBoard.at(sq) == Ranks::Two && board->pieces.at(sq + 20) == Pieces::EMPTY) {
                    addNonCaptureMove(Move((Squares) sq, (Squares) (sq + 20), Pieces::EMPTY, false, true));
                }
            }

            //capturing moves
            if (!board->isSqOffBoard((Squares)(sq + 9)) && pieceColor[board->pieces.at((sq + 9))] == Side::Black) {
                insertwPCaptureMove(Move((Squares) sq, (Squares) (sq + 9), board->pieces.at(sq + 9)));
            }
            if (!board->isSqOffBoard((Squares)(sq + 11)) && pieceColor[board->pieces.at((sq + 11))] == Side::Black) {
                insertwPCaptureMove(Move((Squares) sq, (Squares)(sq+11), board->pieces.at(sq+11)));
            }

            //enPassant
            if(board->enPas != Squares::NO_SQ) {
                if (sq + 9 == board->enPas) {
                    addEnPassantMove(Move((Squares) sq, (Squares) (sq + 9), board->pieces.at(sq + 9), true));
                }
                if (sq + 11 == board->enPas) {
                    addEnPassantMove(Move((Squares) sq, (Squares) (sq + 11), board->pieces.at(sq + 11), true));
                }
            }
        }

        //white rooks
        for (int pceNum = 0; pceNum < board->piecesNum.at(Pieces::wR); pceNum++) {
            sq = board->getFromPiecesList(Pieces::wR, pceNum);
            for (auto dir : rkDir) {
                tSq = sq + dir;
                while (board->pieces.at(tSq) != Squares::OFF_BOARD) {
                    //quite moves
                    if (board->pieces.at(tSq) == Pieces::EMPTY) {
                        addNonCaptureMove(Move((Squares) sq, (Squares) (tSq)));
                    }
                    //capture move
                    if (pieceColor[board->pieces.at(tSq)] == Side::Black) {
                        addCaptureMove(Move((Squares) sq, (Squares) (tSq), board->pieces.at(tSq)));
                        break;
                    }
                    if (pieceColor[board->pieces.at(tSq)] == Side::White)
                        break;
                    tSq += dir;
                }
            }
        }

        //white Queen
        for (int pceNum = 0; pceNum < board->piecesNum.at(Pieces::wQ); pceNum++) {
            sq = board->getFromPiecesList(Pieces::wQ, pceNum);
            for (auto dir : rkDir) {
                tSq = sq + dir;
                while (board->pieces.at(tSq) != Squares::OFF_BOARD) {
                    //quite moves
                    if (board->pieces.at(tSq) == Pieces::EMPTY) {
                        addNonCaptureMove(Move((Squares) sq, (Squares) (tSq)));
                        //break;
                    }
                    //capture move
                    if (pieceColor[board->pieces.at(tSq)] == Side::Black) {
                        addCaptureMove(Move((Squares) sq, (Squares) (tSq), board->pieces.at(tSq)));
                        break;
                    }
                    if (pieceColor[board->pieces.at(tSq)] == Side::White)
                        break;
                    tSq += dir;
                }
            }
        }
        //white Queens diagonal move
        for (int pceNum = 0; pceNum < board->piecesNum.at(Pieces::wQ); pceNum++) {
            sq = board->getFromPiecesList(Pieces::wQ, pceNum);
            for (auto dir : biDir) {
                tSq = sq + dir;
                while (board->pieces.at(tSq) != Squares::OFF_BOARD) {
                    //quite moves
                    if (board->pieces.at(tSq) == Pieces::EMPTY) {
                        addNonCaptureMove(Move((Squares) sq, (Squares) (tSq)));
                    }
                    //capture move
                    if (pieceColor[board->pieces.at(tSq)] == Side::Black) {
                        addCaptureMove(Move((Squares) sq, (Squares) (tSq), board->pieces.at(tSq)));
                        break;
                    }
                    if (pieceColor[board->pieces.at(tSq)] == Side::White)
                        break;
                    tSq += dir;
                }
            }
        }

        //white Bishop
        for (int pceNum = 0; pceNum < board->piecesNum.at(Pieces::wB); pceNum++) {
            sq = board->getFromPiecesList(Pieces::wB, pceNum);
            for (auto dir : biDir) {
                tSq = sq + dir;
                while (board->pieces.at(tSq) != Squares::OFF_BOARD) {
                    //quite moves
                    if (board->pieces.at(tSq) == Pieces::EMPTY) {
                        addNonCaptureMove(Move((Squares) sq, (Squares) (tSq)));
                    }
                    //capture move
                    if (pieceColor[board->pieces.at(tSq)] == Side::Black) {
                        addCaptureMove(Move((Squares) sq, (Squares) (tSq), board->pieces.at(tSq)));
                        break;
                    }
                    if (pieceColor[board->pieces.at(tSq)] == Side::White)
                        break;
                    tSq += dir;
                }
            }
        }

        //white Knight
        for (int pceNum = 0; pceNum < board->piecesNum.at(Pieces::wN); pceNum++) {
            sq = board->getFromPiecesList(Pieces::wN, pceNum);
            for (auto dir : knDir) {
                tSq = sq + dir;
                while (board->pieces.at(tSq) != Squares::OFF_BOARD) {
                    if (board->pieces.at(tSq) == Pieces::EMPTY) {
                        addNonCaptureMove(Move((Squares) sq, (Squares) (tSq)));
                        break;
                    }
                    //capture move
                    if (pieceColor[board->pieces.at(tSq)] == Side::Black) {
                        addCaptureMove(Move((Squares) sq, (Squares) (tSq), board->pieces.at(tSq)));
                        break;
                    }
                    break;
                }
            }
        }
        //white King
        for (int pceNum = 0; pceNum < board->piecesNum.at(Pieces::wK); pceNum++) {
            sq = board->getFromPiecesList(Pieces::wK, pceNum);
            for (auto dir : kiDir) {
                tSq = sq + dir;
                while (board->pieces.at(tSq) != Squares::OFF_BOARD) {
                    if (board->pieces.at(tSq) == Pieces::EMPTY) {
                        addNonCaptureMove(Move((Squares) sq, (Squares) (tSq)));
                        break;
                    }
                    //capture move
                    if (pieceColor[board->pieces.at(tSq)] == Side::Black) {
                        addCaptureMove(Move((Squares) sq, (Squares) (tSq), board->pieces.at(tSq)));
                        break;
                    }
                    break;
                }
            }
        }

        //castling king side
        if (board->castlePerm & Castling::WK) {
            /* if (board->isSquareAttacked(Squares::E1, Side::Black) == std::make_tuple(false, 0) && */
            if (! board->isSquareAttackedOptm(Squares::E1, Side::Black) &&
                    ! board->isSquareAttackedOptm(Squares::F1, Side::Black) &&
                    ! board->isSquareAttackedOptm(Squares::G1, Side::Black)) {
                if (board->pieces.at(Squares::F1) == Pieces::EMPTY && board->pieces.at(Squares::G1) == Pieces::EMPTY) {
                    addNonCaptureMove(Move(Squares::E1, Squares::G1,
                                Pieces::EMPTY, false, false, Pieces::EMPTY, true));
                }
            }
        }

        //castling queen side
        if (board->castlePerm & Castling::WQ) {
            if (! board->isSquareAttackedOptm(Squares::E1, Side::Black) &&
                    ! board->isSquareAttackedOptm(Squares::D1, Side::Black) &&
                    ! board->isSquareAttackedOptm(Squares::C1, Side::Black)) {
                if (board->pieces.at(Squares::D1) == Pieces::EMPTY &&
                        board->pieces.at(Squares::C1) == Pieces::EMPTY && board->pieces.at(Squares::B1) == Pieces::EMPTY) {
                    addNonCaptureMove(Move(Squares::E1, Squares::C1,
                                Pieces::EMPTY, false, false, Pieces::EMPTY, true));
                }
            }
        }
    }
    //Black pieces
    else {
        for (int pceNum = 0; pceNum < board->piecesNum.at(Pieces::bP); pceNum++) {
            sq = board->getFromPiecesList(Pieces::bP, pceNum);
            /* assert(sq != Squares::OFF_BOARD); */

            //not capturing moves
            if (board->pieces.at(sq - 10) == Pieces::EMPTY && board->pieces.at(sq - 10) != Squares::OFF_BOARD) {
                insertbPMove(Move((Squares) sq, (Squares) (sq - 10)));
                if (ranksBoard.at(sq) == Ranks::Seven && board->pieces.at(sq - 20) == Pieces::EMPTY &&
                        board->pieces.at(sq - 20) != (Pieces) Squares::OFF_BOARD) {
                    addNonCaptureMove(Move((Squares) sq, (Squares) (sq - 20), Pieces::EMPTY, false, true));
                }
            }

            //capturing moves
            if (pieceColor[board->pieces.at((sq - 9))] == Side::White &&
                    board->pieces.at(sq - 9) != (Pieces)Squares::OFF_BOARD) {
                insertbPCaptureMove(Move((Squares) sq, (Squares) (sq - 9), board->pieces.at(sq - 9)));
            }
            if (pieceColor[board->pieces.at((sq - 11))] == Side::White &&
                    board->pieces.at(sq - 11) != (Pieces) Squares::OFF_BOARD) {
                insertbPCaptureMove(Move((Squares) sq, (Squares) (sq - 11), board->pieces.at(sq - 11)));
            }

            //enPassant
            if(board->enPas != Squares::NO_SQ) {
                if (sq - 9 == board->enPas) {
                    addEnPassantMove(Move((Squares) sq, (Squares) (sq - 9), board->pieces.at(sq - 9), true));
                }
                if (sq - 11 == board->enPas) {
                    addEnPassantMove(Move((Squares) sq, (Squares) (sq - 11), board->pieces.at(sq - 11), true));
                }
            }
        }

        //black rooks
        for (int pceNum = 0; pceNum < board->piecesNum.at(Pieces::bR); pceNum++) {
            sq = board->getFromPiecesList(Pieces::bR, pceNum);
            for (auto dir : rkDir) {
                tSq = sq + dir;
                while (board->pieces.at(tSq) != Squares::OFF_BOARD) {
                    //quite moves
                    if (board->pieces.at(tSq) == Pieces::EMPTY) {
                        addNonCaptureMove(Move((Squares) sq, (Squares) (tSq)));
                        //break;
                    }
                    //capture move
                    if (pieceColor[board->pieces.at(tSq)] == Side::White) {
                        addCaptureMove(Move((Squares) sq, (Squares) (tSq), board->pieces.at(tSq)));
                        break;
                    }
                    if(pieceColor[board->pieces.at(tSq)] == Side::Black)
                        break;
                    tSq += dir;
                }
            }
        }

        //black Queen
        for (int pceNum = 0; pceNum < board->piecesNum.at(Pieces::bQ); pceNum++) {
            sq = board->getFromPiecesList(Pieces::bQ, pceNum);
            for (auto dir : rkDir) {
                tSq = sq + dir;
                while (board->pieces.at(tSq) != Squares::OFF_BOARD) {
                    //quite moves
                    if (board->pieces.at(tSq) == Pieces::EMPTY) {
                        addNonCaptureMove(Move((Squares) sq, (Squares) (tSq)));
                        //break;
                    }
                    //capture move
                    if (pieceColor[board->pieces.at(tSq)] == Side::White) {
                        addCaptureMove(Move((Squares) sq, (Squares) (tSq), board->pieces.at(tSq)));
                        break;
                    }
                    if(pieceColor[board->pieces.at(tSq)] == Side::Black)
                        break;
                    tSq += dir;
                }
            }
        }
        //black Queens diagonal move
        for (int pceNum = 0; pceNum < board->piecesNum.at(Pieces::bQ); pceNum++) {
            sq = board->getFromPiecesList(Pieces::bQ, pceNum);
            for (auto dir : biDir) {
                tSq = sq + dir;
                while (board->pieces.at(tSq) != Squares::OFF_BOARD) {
                    //quite moves
                    if (board->pieces.at(tSq) == Pieces::EMPTY) {
                        addNonCaptureMove(Move((Squares) sq, (Squares) (tSq)));
                        //break;
                    }
                    //capture move
                    if (pieceColor[board->pieces.at(tSq)] == Side::White) {
                        addCaptureMove(Move((Squares) sq, (Squares) (tSq), board->pieces.at(tSq)));
                        break;
                    }
                    if(pieceColor[board->pieces.at(tSq)] == Side::Black)
                        break;
                    tSq += dir;
                }
            }
        }

        //black Bishop
        for (int pceNum = 0; pceNum < board->piecesNum.at(Pieces::bB); pceNum++) {
            sq = board->getFromPiecesList(Pieces::bB, pceNum);
            for (auto dir : biDir) {
                tSq = sq + dir;
                while (board->pieces.at(tSq) != Squares::OFF_BOARD) {
                    //quite moves
                    if (board->pieces.at(tSq) == Pieces::EMPTY) {
                        addNonCaptureMove(Move((Squares) sq, (Squares) (tSq)));
                        //break;
                    }
                    //capture move
                    if (pieceColor[board->pieces.at(tSq)] == Side::White) {
                        addCaptureMove(Move((Squares) sq, (Squares) (tSq), board->pieces.at(tSq)));
                        break;
                    }
                    if(pieceColor[board->pieces.at(tSq)] == Side::Black)
                        break;
                    tSq += dir;
                }
            }
        }

        //black Knight
        for(int pceNum = 0; pceNum < board->piecesNum.at(Pieces::bN); pceNum++) {
            sq = board->getFromPiecesList(Pieces::bN, pceNum);
            for(auto dir : knDir) {
                tSq = sq + dir;
                while (board->pieces.at(tSq) != Squares::OFF_BOARD) {
                    if(board->pieces.at(tSq) == Pieces::EMPTY) {
                        addNonCaptureMove(Move((Squares) sq, (Squares) (tSq)));
                        break;
                    }
                    //capture move
                    if (pieceColor[board->pieces.at(tSq)] == Side::White) {
                        addCaptureMove(Move((Squares) sq, (Squares) (tSq), board->pieces.at(tSq)));
                        break;
                    }
                    break;
                }
            }
        }
        //black King
        for(int pceNum = 0; pceNum < board->piecesNum.at(Pieces::bK); pceNum++) {
            sq = board->getFromPiecesList(Pieces::bK, pceNum);
            for(auto dir : kiDir) {
                tSq = sq + dir;
                while (board->pieces.at(tSq) != Squares::OFF_BOARD) {
                    if(board->pieces.at(tSq) == Pieces::EMPTY) {
                        addNonCaptureMove(Move((Squares) sq, (Squares) (tSq)));
                        break;
                    }
                    //capture move
                    if (pieceColor[board->pieces.at(tSq)] == Side::White) {
                        addCaptureMove(Move((Squares) sq, (Squares) (tSq), board->pieces.at(tSq)));
                        break;
                    }
                    break;
                }
            }
        }

        //castling king side
        if (board->castlePerm & Castling::BK) {
            if (! board->isSquareAttackedOptm(Squares::E8, Side::White) &&
                    ! board->isSquareAttackedOptm(Squares::F8, Side::White) &&
                    ! board->isSquareAttackedOptm(Squares::G8, Side::White)) {
                if (board->pieces.at(Squares::F8) == Pieces::EMPTY && board->pieces.at(Squares::G8) == Pieces::EMPTY) {
                    addNonCaptureMove(Move(Squares::E8, Squares::G8,
                                Pieces::EMPTY, false, false, Pieces::EMPTY, true));
                }
            }
        }

        //castling queen side
        if (board->castlePerm & Castling::BQ) {
            if (! board->isSquareAttackedOptm(Squares::E8, Side::White) &&
                    ! board->isSquareAttackedOptm(Squares::D8, Side::White) &&
                    ! board->isSquareAttackedOptm(Squares::C8, Side::White)) {
                if (board->pieces.at(Squares::D8) == Pieces::EMPTY &&
                        board->pieces.at(Squares::C8) == Pieces::EMPTY && board->pieces.at(Squares::B8) == Pieces::EMPTY) {
                    addNonCaptureMove(Move(Squares::E8, Squares::C8,
                                Pieces::EMPTY, false, false, Pieces::EMPTY, true));
                }
            }
        }
    }

    std::sort(std::begin(moves), std::end(moves),[](Move a, Move b){ return a.score > b.score;});
}

void MoveGenerator::insertwPCaptureMove(Move move) {
    if(ranksBoard.at(move.from) == Ranks::Seven) {
        move.promoted = Pieces::wQ;
        addCaptureMove(move);

        move.promoted = Pieces::wR;
        addCaptureMove(move);

        move.promoted = Pieces::wB;
        addCaptureMove(move);

        move.promoted = Pieces::wN;
        addCaptureMove(move);
    }
    else {
        move.promoted = Pieces::EMPTY;
        addCaptureMove(move);
    }
}

void MoveGenerator::insertwPMove(Move move) {

    if(ranksBoard.at(move.from) == Ranks::Seven) {
        move.captured = Pieces::EMPTY;

        move.promoted = Pieces::wQ;
        addNonCaptureMove(move);

        move.promoted = Pieces::wR;
        addNonCaptureMove(move);

        move.promoted = Pieces::wB;
        addNonCaptureMove(move);

        move.promoted = Pieces::wN;
        addNonCaptureMove(move);
    }
    else {
        move.promoted = Pieces::EMPTY;
        addNonCaptureMove(move);
    }
}

void MoveGenerator::printAllMoves() {
    for(auto i : moves)
        std::cout << i << "\tScore : " << i.score << "\n";
}

void MoveGenerator::insertbPCaptureMove(Move move) {
    if(ranksBoard.at(move.from) == Ranks::Two) {
        move.promoted = Pieces::bQ;
        addCaptureMove(move);

        move.promoted = Pieces::bR;
        addCaptureMove(move);

        move.promoted = Pieces::bB;
        addCaptureMove(move);

        move.promoted = Pieces::bN;
        addCaptureMove(move);
    }
    else {
        move.promoted = Pieces::EMPTY;
        addCaptureMove(move);
    }
}

void MoveGenerator::insertbPMove(Move move) {

    if(ranksBoard.at(move.from) == Ranks::Two) {
        move.captured = Pieces::EMPTY;

        move.promoted = Pieces::bQ;
        addNonCaptureMove(move);

        move.promoted = Pieces::bR;
        addNonCaptureMove(move);

        move.promoted = Pieces::bB;
        addNonCaptureMove(move);

        move.promoted = Pieces::bN;
        addNonCaptureMove(move);
    }
    else {
        move.promoted = Pieces::EMPTY;
        addNonCaptureMove(move);
    }
}

Move MoveGenerator::readMove(std::string moveStr) {
    if(moveStr.at(1) > '8' || moveStr.at(1) < '1') return InvalidMove();
    if(moveStr.at(3) > '8' || moveStr.at(3) < '1') return InvalidMove();
    if(moveStr.at(0) > 'h' || moveStr.at(0) < 'a') return InvalidMove();
    if(moveStr.at(2) > 'h' || moveStr.at(2) < 'a') return InvalidMove();

    Squares from = FRtoSq(Files(moveStr.at(0) - 'a'), Ranks(moveStr.at(1) - '1'));
    Squares to = FRtoSq(Files(moveStr.at(2) - 'a'), Ranks(moveStr.at(3) - '1'));

    generateAllMoves();
    for(auto move : moves) {
        if(move.from == from && move.to == to) {
            if(move.promoted != Pieces::EMPTY) {

                if((move.promoted == Pieces::wR || move.promoted == Pieces::bR) && moveStr.at(4) == 'r')
                    return move;
                else if((move.promoted == Pieces::wB || move.promoted == Pieces::bB) && moveStr.at(4) == 'b')
                    return move;
                else if((move.promoted == Pieces::wQ || move.promoted == Pieces::bQ) && moveStr.at(4) == 'q')
                    return move;
                else if((move.promoted == Pieces::wN || move.promoted == Pieces::bN) && moveStr.at(4) == 'n')
                    return move;

                continue;
            }
            return move;
        }
    }
    return InvalidMove();
}

bool MoveGenerator::moveExists(const Move& move) {

    generateAllMoves();
    for(auto iMove : moves) {
        if(!board->makeMove(iMove)) continue;

        board->undoMove();
        if(move == iMove) return true;
    }

    return false;
}

void MoveGenerator::perft(Board board, int depth) {

    if(depth == 0) {
        leafNodes++;
        return;
    }

    MoveGenerator gen(board);
    gen.generateAllMoves();

    for(auto move : gen.moves) {
        if(!board.makeMove(move)) continue;

        gen.perft(board, depth-1);
        board.undoMove();
    }

    return;
}

void MoveGenerator::perftTest(const Board& board, int depth) {

    perft(board, depth);
    std::cout << "Test complete, The number of calculated positions: " << std::dec << leafNodes << "\n";

    return;
}

MoveGenerator::MoveGenerator(Board &board) : board(&board) {
    moves.reserve(2000);
    capMoves.reserve(2000);
    if(set) return;
    set = true;

    for(Pieces att = Pieces::wP; att <= Pieces::bK; att++)
        for(Pieces vic = wP; vic <= Pieces::bK; vic++)
            mvvLva[vic][att] = capScore[vic] + 6 - (capScore[att] / 100);
}

void MoveGenerator::generateAllCapMoves() {
    generateAllMoves();

    for(auto move : moves)
        if(move.captured != Pieces::EMPTY)
            capMoves.emplace_back(move);
}
