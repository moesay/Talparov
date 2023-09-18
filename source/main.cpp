#include "../include/talparov.h"
#include "../include/utils.h"
/* #include "../include/movegen.h" */
/* #include "../include/board.h" */
/* #include "../include/misc.h" */
/* #include "../include/search.h" */
/* #include "/home/moe//benchmarking/bm.h" */
#define FEN_1 "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define FEN_2 "rnbkqbnr/5p1p/2p5/3p2p1/ppNp4/2P5/1P1PN1PP/R11QKBNR w KQkq d6 0 1"
#define FEN_3 "r3kr2/8/8/8/8/8/8/R3K2R w KQkq - 0 1"
#define FEN_4 "r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/2N2N2/PPPP1PPP/R1BQK2R w KQkq - 5 4"
#define FEN_5 "8/8/1B6/7b/7k/8/2B1b3/7K w - - 0 1"
#define FEN_6 "r1bqk2r/pp3ppp/2pb1n2/3pp3/1n1PP3/P1N1BP1N/1PP3PP/R2QKB1R w KQkq - 1 8"

#define TEST1 "2rr3k/pp3pp1/1nnqbN1p/3pN3/2pP4/2P3Q1/PPB4P/R4RK1 w - -"
#define TEST2 "r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w KQkq - 0 1"
#define TEST3 "3r2k1/2Q2ppp/p1p2n2/2P1p3/2BqP3/8/5PPP/1R4K1 w - - 0 1"
#define TEST4 "2Q5/1R3ppk/p4n2/2Pqp2p/8/8/5PPP/6K1 b - - 1 5"
class temp {
    bool enPassant;
    bool pawnStart;
    bool castling;
    bool valid;
    int move;

    int score;
};

auto main() -> int {
    Talparov eng;
    initAll();
    eng.UCILoop();
}