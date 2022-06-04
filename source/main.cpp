#include "../include/utils.h"
#include "../include/movegen.h"
#include "../include/board.h"
#include "../include/misc.h"
#include "../include/search.h"

auto main() -> int {
    initAll();
    std::string moveIn;
    Board brd;
    brd.loadFEN(TEST);
    MoveGenerator gen(brd);
    Search srch;

    while(true) {
        brd.print();
        std::cout << "\n[q]uit, [c]alculate, [u]ndo, Or play a move: ";
        std::cin >> moveIn;
        if(moveIn.at(0) == 'q')
            break;
        else if(moveIn.at(0) == 'u') {
            brd.undoMove();
            continue;
        }
        else if(moveIn.at(0) == 'c' && moveIn.length() == 1) {
                srch.info.depth = 10;
                srch.calculate(brd);
        }
        else {
            auto move = gen.readMove(moveIn);
            if(move.valid == true) {
                brd.makeMove(move);
            }
            else
                std::cout << "\n\nInvalid Move\n\n";
        }
    }

    return 0;
}