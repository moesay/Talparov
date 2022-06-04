//
// Created by moe on 5/9/22.
//

#ifndef TALPAROV_PV_H
#define TALPAROV_PV_H

#include <unordered_map>
#include "consts.h"

#include "move.h"

struct PV {
    u64 posKey;
    Move move;
};

class PVTable {
private:
    std::unordered_map<int, PV> table;
public:
    void clear();
    void addPvMove(const Move&, const u64&);
    Move getPvMove(const u64&);
};

#endif //TALPAROV_PV_H
