#include "../include/pv.h"
#include <cassert>

void PVTable::clear() {
    table.clear();
}

void PVTable::addPvMove(const Move& move, const u64& key) {
    int index = key;

    table[index].move = move;
    table[index].posKey = key;
};

Move PVTable::getPvMove(const u64& key) {
    int index = key;

    if(key == table[index].posKey)
        return table[index].move;

    return InvalidMove();
}
