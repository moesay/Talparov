//
// Created by moe on 8/13/23.
//

#ifndef TALPAROV_MAIN_H
#define TALPAROV_MAIN_H
#define INPUTBUFFER 400 * 6

#include "defs.h"
#include "consts.h"
#include "utils.h"
#include "movegen.h"
#include "board.h"
#include "misc.h"
#include "search.h"
#include <stdio.h>
#include <string.h>

class Talparov {
private:
    Board board;
    MoveGenerator moveGen;
    Search srch;
    bool init;
public:
    void parseGo(std::string);
    void parsePosition(std::string);

    void UCILoop();
    Talparov();
};

#endif //TALPAROV_MAIN_H
