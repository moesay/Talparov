#include "../include/search.h"
#include <limits>


void Search::clear(Board& board) {
    for(int i = 0; i < 13; i++)
        for(int j = 0; j < SQUARES; j++)
            board.searchHistory[i][j] = 0;

    for(int i = 0; i < 2; i++)
        for(int j = 0; j < MAXDEPTH; j++)
            board.killers[i][j] = InvalidMove();

    board.pvTable.clear();
    board.ply = 0;

    info.nodes = 0;
    info.failHigh = 0;
    info.failHighFirst = 0;
}

int Search::QuieSearch(int alpha, int beta, Board& board) {
    info.nodes++;

    if(board.isRep() || board.fiftyMoves >= 100)
        return 0;

    if(board.ply > MAXDEPTH -1)
        return board.eval();

    int score = board.eval();

    if(score >= beta)
        return beta;

    if(score > alpha)
        alpha = score;


    int legalMoves = 0, oldAlpha = 0;
    Move bestMove;
    Move pvMove = board.pvTable.getPvMove(board.getKey());
    score = -INF;

    MoveGenerator gen(board);
    gen.generateAllCapMoves();

    for(auto move : gen.capMoves) {
        if(!board.makeMove(move)) continue;

        legalMoves++;
        score = -QuieSearch(-beta, -alpha, board);
        board.undoMove();

        if(score > alpha) {
            if(score >= beta) {
                if(legalMoves == 1)
                    info.failHighFirst++;
                info.failHigh++;

                return beta;
            }
            alpha = score;
            bestMove = move;
        }
    }

    if(alpha != oldAlpha)
        board.pvTable.addPvMove(bestMove, board.getKey());

    return alpha;
}

int Search::alphaBeta(int alpha, int beta, int depth, Board& board, bool doNull) {
    if(depth == 0)
        return QuieSearch(alpha, beta, board);

    info.nodes++;

    if(board.isRep() || board.fiftyMoves >= 100)
        return 0;

    if(board.ply > MAXDEPTH -1)
        return board.eval();

    MoveGenerator gen(board);
    gen.generateAllMoves();

    int legalMoves = 0, oldAlpha = 0, score = 0;
    Move bestMove;
    Move pvMove = board.pvTable.getPvMove(board.getKey());

    if(pvMove.valid == true && pvMove.from != Squares::NO_SQ) {
        for(auto &move : gen.moves) {
            if(move == pvMove)
                move.score = 2000000;
            break;
        }
    }

    for(auto move : gen.moves) {
        if(!board.makeMove(move)) continue;

        legalMoves++;
        score = -alphaBeta(-beta, -alpha, depth-1, board, doNull);
        board.undoMove();

        if(score > alpha) {
            if(score >= beta) {
                if(legalMoves == 1)
                    info.failHighFirst++;
                info.failHigh++;

                if(move.captured == Pieces::EMPTY) {
                    board.killers[1][board.ply] = board.killers[0][board.ply];
                    board.killers[0][board.ply] = move;
                }
                return beta;
            }
            alpha = score;
            bestMove = move;

            if(move.captured == Pieces::EMPTY)
                board.searchHistory[board.pieces.at(bestMove.from)][bestMove.to] += depth;
        }
    }

    if(legalMoves == 0) {
    Side temp = (board.side == Side::White ? Side::Black : Side::White);
    auto [attacked, noOfAttackers] = board.isSquareAttacked(board.kingSq.at(board.side), temp);
        if(attacked)
            return -MATE + board.ply;
        else
            return 0;
    }

    if(alpha != oldAlpha)
        board.pvTable.addPvMove(bestMove, board.getKey());

    return alpha;
}

void Search::calculate(Board& board) {
    Move bestMove = InvalidMove();
    int bestScore = -INF;
    int pvMoves = 0;

    clear(board);

    for(int currDepth = 1; currDepth <= info.depth; currDepth++) {
        bestScore = alphaBeta(-INF, INF, currDepth, board, true);
        pvMoves = getPvLine(currDepth, board);
        bestMove = board.pvArray.at(0);

        std::cout << std::dec << "Depth: " << currDepth << " " <<
            "Nodes: " << info.nodes << "\n" <<
            "Score: " << bestScore << " " <<
            "Best Move: " << bestMove << "\n";

        pvMoves = getPvLine(currDepth, board);
        std::cout << "Principal Variation: ";
        for(int i = 0; i < pvMoves; i++)
            std::cout << board.pvArray.at(i) << " ";

        std::cout << std::endl;

        std::cout << "Ordering Eff. :" << (info.failHighFirst/info.failHigh) * 100 << "%\n\n";

    }
}
