//
// Created by moe on 5/9/22.
//

#ifndef TALPAROV_TPTABLE_H
#define TALPAROV_TPTABLE_H

#include <unordered_map>
#include "defs.h"
#include "move.h"
#define MATE 29000

class Board;

enum TPFlags {
    None, Alpha, Beta, Exact
};

struct TPEntry {
    std::uint64_t posKey;
    Move move;
    int depth;
    int score;
    int flags;
};

class TranspositionTable {
private:
    std::unordered_map<int, TPEntry> table;
    /* int newWrite; */
    /* int overWrite; */
    int hit;
    int cut;
    Board *board;
public:
    void clear();
    void addPvMove(const Move&, const std::uint64_t&);
    Move getPvMove(const std::uint64_t&);
    void addHashEntry(const Move&, int, TPFlags, const int, const std::uint64_t&, int*);
    bool getHashEntry(int, int, int, std::uint64_t, int*, Move*, int*);

    friend class Search;
};

#endif //TALPAROV_TPTABLE_H
