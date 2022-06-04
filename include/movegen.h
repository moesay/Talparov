//
// Created by moe on 1/28/21.
//

#ifndef TALPAROV_MOVEGEN_H
#define TALPAROV_MOVEGEN_H

#include "move.h"
#include <vector>
#include "board.h"
#include "utils.h"

const int capScore[13] = {0, 100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600};

class MoveGenerator {
private:
    static bool set;
    friend class Search;
    void insertwPCaptureMove(Move);
    void insertbPCaptureMove(Move);
    void insertwPMove(Move);
    void insertbPMove(Move);
    void addNonCaptureMove(Move);
    void addCaptureMove(Move);
    void addEnPassantMove(Move);

    static unsigned long leafNodes;
    void perft(Board, int);
    static int mvvLva[13][13];
    Board *board;
public:
    void generateAllCapMoves();
    void generateAllMoves();
    MoveGenerator(Board&);
    std::vector<Move> moves;
    std::vector<Move> capMoves;
    Move readMove(std::string);

    void printAllMoves();
    void perftTest(const Board&, int);
    bool moveExists(const Move&);
};
#endif //TALPAROV_MOVEGEN_H
