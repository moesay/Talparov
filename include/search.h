#ifndef TALPAROV_SEARCH_H
#define TALPAROV_SEARCH_H

#include "consts.h"
#include "board.h"
#include "utils.h"
#include "move.h"
#include "misc.h"

struct SearchInfo {
    int depth;
    long nodes;

    float failHigh, failHighFirst;
};

class Search {
public:
    void clear(Board&);
    void calculate(Board&);

    int alphaBeta(int, int, int, Board&, bool);
    int QuieSearch(int, int, Board&);

    SearchInfo info;
private:
};

#endif
