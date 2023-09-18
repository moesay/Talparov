#ifndef TALPAROV_SEARCH_H
#define TALPAROV_SEARCH_H

#include "defs.h"
#include "board.h"
#include "utils.h"
#include "move.h"
#include "misc.h"

#ifdef WIN32
#include "windows.h"
#else
#include "sys/time.h"
#include "sys/select.h"
#include "unistd.h"
#include "string.h"
#endif

enum SearchMode {
    Normal, Ponder
};

struct EngineOptions {
    bool ponder;
    int hashTableSize;
};

class Search;
int bufPeek();
void readIoBuf(Search*);

struct SearchInfo {
    time_type startTime, stopTime;
    int depth;
    int depthSet, timeSet;
    int movesToGo;
    bool infinite;

    long nodes;

    bool quit, stopped;

    float failHigh, failHighFirst;
};

class Search {
public:
    void clear(Board&);
    void calculate(Board&);
    void check();

    int negamax(int, int, int, Board&, bool);
    int qSearch(int, int, Board&);

    EngineOptions options;
    SearchMode mode;
    SearchInfo info;
private:
};

#endif
