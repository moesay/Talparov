#include "../include/search.h"
#include <bit>
#include <limits>

#define CHECKMATE 100000

#define INF std::numeric_limits<int>::max()

void Search::check() {
    if((info.timeSet == true && getTime() > info.stopTime) && mode != SearchMode::Ponder)
        info.stopped = true;

    readIoBuf(this);
}

void Search::clear(Board& board) {
    for(int i = 0; i < 13; i++)
        for(int j = 0; j < SQUARES; j++)
            board.searchHistory[i][j] = 0;

    for(int i = 0; i < 2; i++)
        for(int j = 0; j < MAXDEPTH; j++)
            board.killers[i][j] = InvalidMove();

    board.ply = 0;
    /* board.tpTable.overWrite = 0; */
    board.tpTable.hit = 0;
    board.tpTable.cut = 0;


    info.stopped = false;
    info.nodes = 0;
    info.failHigh = 0;
    info.failHighFirst = 0;
}

int Search::qSearch(int alpha, int beta, Board& board) {
    if((info.nodes & 2047) == 0)
        check();

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
    Move pvMove = board.tpTable.getPvMove(board.getKey());
    score = -INF;

    MoveGenerator gen(board);
    gen.generateAllCapMoves();

    for(auto move : gen.capMoves) {
        if(!board.makeMove(move)) continue;

        legalMoves++;
        score = -qSearch(-beta, -alpha, board);
        board.undoMove();

        if(info.stopped == true)
            return 0;

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
        board.tpTable.addPvMove(bestMove, board.getKey());

    return alpha;
}

int Search::negamax(int alpha, int beta, int depth, Board& board, bool nullPruning) {

    if(depth <= 0)
        return qSearch(alpha, beta, board);

    if((info.nodes & 4095) == 0) {
        int tableSizeMB = (board.tpTable.table.size() * sizeof(TPEntry)) / (1_MB);
        if(tableSizeMB > options.hashTableSize)
            board.tpTable.table.clear();

        check();
    }

    info.nodes++;

    if((board.isRep() || board.fiftyMoves >= 100) && board.ply > 0)
        return 0;

    if(board.ply > MAXDEPTH -1)
        return board.eval();

    bool kingInCheck = board.isSquareAttackedOptm(board.kingSq.at(board.side), board.getOppSide());
    if(kingInCheck)
        depth += 1;

    int score = -INF;
    Move pvMove = InvalidMove();

    if(board.tpTable.getHashEntry(alpha, beta, depth, board.getKey(), &board.ply, &pvMove, &score)) {
        board.tpTable.cut++;
        return score;
    }

    if(nullPruning && !kingInCheck && board.ply && (board.bigPieces.at(board.side) > 0) && depth >= 4) {
        board.makeNullMove();
        score = -negamax(-beta, -beta+1, depth - 4, board, false);
        board.undoNullMove();
        if(info.stopped == true)
            return 0;
        if(score >= beta)
            return beta;
    }

    MoveGenerator gen(board);
    gen.generateAllMoves();

    int legalMoves = 0, oldAlpha = alpha;
    Move bestMove;

    int bestScore = -INF;

    if(pvMove.valid && pvMove.from != Squares::NO_SQ) {
        for(auto &move : gen.moves) {
            if(move == pvMove)
                move.score = 2000000;
            break;
        }
    }

    for(auto move : gen.moves) {
        if(!board.makeMove(move)) continue;

        legalMoves++;
        score = -negamax(-beta, -alpha, depth-1, board, nullPruning);
        board.undoMove();

        if(info.stopped == true)
            return 0;

        if(score > bestScore) {
            bestMove = move;
            bestScore = score;
            if(score > alpha) {
                if(score >= beta) {
                    if(legalMoves == 1)
                        info.failHighFirst++;
                    info.failHigh++;

                    if(move.captured == Pieces::EMPTY) {
                        board.killers[1][board.ply] = board.killers[0][board.ply];
                        board.killers[0][board.ply] = move;
                    }

                    board.tpTable.addHashEntry(bestMove, score, TPFlags::Beta, depth, board.getKey(), &board.ply);

                    return beta;
                }
                alpha = score;

                if(move.captured == Pieces::EMPTY)
                    board.searchHistory[board.pieces.at(bestMove.from)][bestMove.to] += depth;
            }
        }
    }

    if(legalMoves == 0) {
        if(kingInCheck)
            return -CHECKMATE + board.ply;
        else
            return 0;
    }

    if(alpha != oldAlpha)
        board.tpTable.addHashEntry(bestMove, bestScore, TPFlags::Exact, depth, board.getKey(), &board.ply);
    else
        board.tpTable.addHashEntry(bestMove, alpha, TPFlags::Alpha, depth, board.getKey(), &board.ply);

    return alpha;
}

void Search::calculate(Board& board) {
    Move bestMove = InvalidMove();
    int bestScore = -INF;
    int pvMoves = 0;

    clear(board);

    for(int currDepth = 1; currDepth <= info.depth; currDepth++) {
        bestScore = negamax(-INF, INF, currDepth, board, true);

        if(info.stopped == true)
            break;

        pvMoves = getPvLine(currDepth, &board);
        bestMove = board.pvArray.at(0);

        double hashfull = ((board.tpTable.table.size() * sizeof(TPEntry)) / 1_MB) / (double)options.hashTableSize;
        std::cout << std::dec << "info score ";
        if(abs(bestScore) > 50000) std::cout << "mate " << (CHECKMATE - abs(bestScore)) * (bestScore / abs(bestScore));
        else std::cout << "cp " << bestScore;
        std::cout << " depth " << currDepth
            << " nodes " << info.nodes
            /* idk why but the uci gui divides the hashtable capacity by 10
             * so to get correct values i mult.d here by 10. (100 to get % and a 10 for that reason)
             * hence the 1000.
             * */
            << " hashfull " << hashfull * 1000
            << " cpuload " << getCPULoad()
            << " time " << std::chrono::duration_cast<std::chrono::milliseconds>(getTime() - info.startTime).count() << " ";

        std::cout << "pv ";
        for(int i = 0; i < pvMoves; i++)
            printf(" %s", board.pvArray.at(i).getMoveStr());
        /* std::cout << " " << board.pvArray.at(i) << std::flush; */

        std::cout << std::endl;
    }

    /* std::cout << "bestmove " << bestMove; */
    printf("bestmove %s", bestMove.getMoveStr());

    if(board.pvArray.size() > 0 && options.ponder){
        std::cout << " ponder " << board.pvArray.at(1);
    }
    std::cout << "\n";
    /* std::cout << std::endl; */
    /* std::cout.flush(); */
}

int bufPeek() {
#ifndef WIN32
    fd_set readfds;
    struct timeval tv;
    FD_ZERO (&readfds);
    FD_SET (fileno(stdin), &readfds);
    tv.tv_sec=0; tv.tv_usec=0;
    select(16, &readfds, 0, 0, &tv);

    return (FD_ISSET(fileno(stdin), &readfds));
#else
    static int init = 0, pipe;
    static HANDLE inh;
    DWORD dw;

    if (!init) {
        init = 1;
        inh = GetStdHandle(STD_INPUT_HANDLE);
        pipe = !GetConsoleMode(inh, &dw);
        if (!pipe) {
            SetConsoleMode(inh, dw & ~(ENABLE_MOUSE_INPUT|ENABLE_WINDOW_INPUT));
            FlushConsoleInputBuffer(inh);
        }
    }
    if (pipe) {
        if (!PeekNamedPipe(inh, NULL, 0, NULL, &dw, NULL)) return 1;
        return dw;
    } else {
        GetNumberOfConsoleInputEvents(inh, &dw);
        return dw <= 1 ? 0 : dw;
    }
#endif
}

void readIoBuf(Search *srch) {
    std::string input;

    if(bufPeek()) {
        std::getline(std::cin, input);

        if(input.find("quit") != std::string::npos) {
            srch->info.quit = true;
        }
        if(input.find("stop") != std::string::npos) {
            srch->info.stopped = true;
        }
        if(input.find("isready") != std::string::npos) {
            std::cout << "readyok\n";
        }
        if(input.find("ponderhit") != std::string::npos) {
            srch->mode = SearchMode::Normal;
        }
    }
}
