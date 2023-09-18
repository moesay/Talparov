//
// Created by moe on 8/13/23.
//

#include "../include/talparov.h"
#include <sstream>
#include <string>

Talparov::Talparov() : moveGen(this->board) {
    srch.options.hashTableSize = 128;
    srch.options.ponder = false;
    srch.mode = SearchMode::Normal;
    init = false;
}

void Talparov::UCILoop() {

    std::setbuf(stdin, nullptr);
    std::setbuf(stdout, nullptr);

    std::cout.setf(std::ios::unitbuf);

    std::string input;

    std::cout << "id name Talparov" << std::endl;
    std::cout << "id author Mohamed ElSayed" << std::endl;

    std::cout << "option name Clear Hash type button" << std::endl;
    std::cout << "option name Hash type spin default 128 min 24 max 512" << std::endl;
    std::cout << "option name Ponder type check default false" << std::endl;
    std::cout << "uciok" << std::endl;

    while(std::getline(std::cin, input)) {
        std::cout.flush();

        if(input.empty())
            continue;

        if(input.find("isready") != std::string::npos)
            std::cout << "readyok" << std::endl;

        else if(input.find("position") != std::string::npos)
            parsePosition(input);

        else if(input.find("ucinewgame") != std::string::npos)
            parsePosition("position startpos");

        else if(input.find("go") != std::string::npos) {
            parseGo(input);
        }

        else if(input.find("uci") != std::string::npos) {
            std::cout << "id name Talparov" << std::endl;
            std::cout << "id author Mohamed ElSayed" << std::endl;

            std::cout << "option name Clear Hash type button" << std::endl;
            std::cout << "option name Hash type spin default 128 min 24 max 512" << std::endl;
            std::cout << "option name Ponder type check default false" << std::endl;
            std::cout << "uciok" << std::endl;
        }

        else if(input.find("quit") != std::string::npos) {
            srch.info.quit = true;
            break;
        }

        else if(input.find("print") != std::string::npos)
            board.print();

        else if(input.find("eval") != std::string::npos)
            std::cout << board.eval() << std::endl;

        else if(input.find("stop") != std::string::npos)
            srch.info.stopped = true;

        else if(input.find("setoption name Hash value ") != std::string::npos) {
            srch.options.hashTableSize = std::atoi(input.substr(26).c_str());
            if(srch.options.hashTableSize < 24) srch.options.hashTableSize = 24;
            if(srch.options.hashTableSize > 512) srch.options.hashTableSize = 512;
            std::cout << "hash table size set to " << srch.options.hashTableSize << " mb"<< std::endl;
        }

        else if(input.find("setoption name Ponder value true") != std::string::npos) {
            srch.options.ponder = true;
            std::cout << "ponder set to true\n";
        }

        else if(input.find("setoption name Ponder value false") != std::string::npos) {
            srch.options.ponder = false;
            std::cout << "ponder set to false\n";
        }

        else if(input.find("setoption name Clear Hash") != std::string::npos) {
            board.tpTable.clear();
            std::cout << "hash table cleared\n";
        }

        else
            std::cout << "what?\n";

        if(srch.info.quit) break;
    }
}

void Talparov::parsePosition(std::string line) {
    init = true;
    std::size_t t;
    Move move;
    std::stringstream ss;

    line.erase(0, 9);

    if(line.find("startpos") != std::string::npos) {
        board.loadFEN(START_FEN);
    }
    else {
        t = line.find("fen");
        if(t == std::string::npos)
            board.loadFEN(START_FEN);
        else {
            t += 4;
            board.loadFEN(line.substr(t));
        }
    }

    t = line.find("moves");

    if(t != std::string::npos) {
        line = line.substr(t+6);
        ss << line;
        while(ss >> line) {
            move = moveGen.readMove(line);
            if(!move.valid) break;
            board.makeMove(move);
        }
    }
    board.ply = 0;
}

void Talparov::parseGo(std::string line) {
    if(!init) board.loadFEN(START_FEN);
    int depth = -1, movestogo = 30, movetime = -1, inc = 0;
    long time = -1;
    std::size_t t;
    std::stringstream ss(line);

    if(line.find("ponder") != std::string::npos)
        srch.mode = SearchMode::Ponder;
    else
        srch.mode = SearchMode::Normal;

    srch.info.timeSet = false;
    while(ss >> line) {
        if(line == "infinite") {
            ;
        }
        if((line == "winc") && board.getSide() == Side::White) {
            ss >> line;
            inc = atoi(line.data());
        }
        if((line == "binc") && board.getSide() == Side::Black) {
            ss >> line;
            inc = atoi(line.data());
        }
        if((line == "wtime") && board.getSide() == Side::White) {
            ss >> line;
            time = atoi(line.data());
        }
        if((line == "btime") && board.getSide() == Side::Black) {
            ss >> line;
            time = atoi(line.data());
        }
        if(line == "movestogo") {
            ss >> line;
            movestogo = atoi(line.data());
        }
        if(line == "movetime") {
            ss >> line;
            movetime = atoi(line.data());
        }
        if(line == "depth") {
            ss >> line;
            depth = atoi(line.data());
        }
    }

    if(movetime != -1) {
        time = movetime;
        movestogo = 1;
    }

    srch.info.startTime = getTime();
    srch.info.depth = depth;

    if(time != -1) {
        srch.info.timeSet = true;
        time /= movestogo;
        srch.info.stopTime = srch.info.startTime + int_to_ms(time + inc);
    }

    if(depth == -1)
        srch.info.depth = MAXDEPTH;

    if(time != -1)
        std::cout << "Time: " << time
            << " Start time : " << srch.info.startTime
            << " Stop time: " << srch.info.stopTime << " ";

    std::cout << "Depth: " << srch.info.depth
        << " Time is configured? " << (srch.info.timeSet ? "Yes" : "No") << std::endl;

    srch.calculate(this->board);
}
