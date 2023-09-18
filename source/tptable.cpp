#include "../include/tptable.h"
#include <cassert>
#include <cstdio>

void TranspositionTable::clear() {
    table.clear();
}

void TranspositionTable::addPvMove(const Move& move, const std::uint64_t& key) {
    if(move.from == Squares::NO_SQ || move.to == Squares::NO_SQ) return;
    int index = key;
    table[index].move = move;
    table[index].posKey = key;
};

Move TranspositionTable::getPvMove(const std::uint64_t& key) {
    if(table.find(key) == table.end()) return InvalidMove();
    int index = key;
    return table[index].move;

    return InvalidMove();
}

void TranspositionTable::addHashEntry(const Move& move, int score, TPFlags flags, const int depth, const std::uint64_t& key, int *ply) {
    if(move.from == Squares::NO_SQ || move.to == Squares::NO_SQ) return;
    int index = key;

    /* if(table[index].posKey == 0) */
    /*     newWrite++; */
    /* else */
    /*     overWrite++; */

    if(score > MATE) score += *ply;
    else if(score < -MATE) score -= *ply;

    table[index].move = move;
    table[index].posKey = key;
    table[index].flags = flags;
    table[index].score = score;
    table[index].depth = depth;
}

bool TranspositionTable::getHashEntry(int alpha, int beta, int depth, std::uint64_t key, int *ply, Move *move, int *score) {
    if(table.find(key) == table.end()) return false;
    int index = key;

    *move = table[index].move;
    if(table[index].depth >= depth) {
        this->hit++;
        *score = table[index].score;

        if(*score > MATE) *score -= *ply;
        else if(*score < -MATE) *score += *ply;

        switch(table[index].flags) {
            case TPFlags::Alpha:
                if(*score <= alpha) {
                    *score = alpha;
                    return true;
                }
                break;
            case TPFlags::Beta:
                if(*score >= beta) {
                    *score = beta;
                    return true;
                }
                break;
            case TPFlags::Exact:
                return true;
                break;
            default:
                assert(false);
                break;
        }
    }
    return false;
}
