#include "../include/misc.h"

int getPvLine(const int& depth, Board* board) {
    Move move = board->tpTable.getPvMove(board->getKey());

    MoveGenerator gen(*board);

    int count = 0;
    while(move.valid == true && count < depth && move.to != Squares::NO_SQ) {
        if(gen.moveExists(move)) {
            board->makeMove(move);
            board->pvArray.at(count++) = move;
        }
        else
            break;

        move = board->tpTable.getPvMove(board->getKey());
    }

    while(board->ply > 0)
        board->undoMove();

    return count;
}

