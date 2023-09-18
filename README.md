# Talparov
Talpavor, Tal-(Kas)parov, is a piece-centric experimental chess engine built with one thing in mind. Getting it to work!

Its ELO is around 1500 so it shouldn't be used to analyze real chess games, but Its source code is clear and self-explanatory so you can use it as a reference to build a stronger engine.

Talparov is a bit slow, the move ordering is not perfect so the engine is not making the best of null move pruning.
Moves are represented as objects for easier debugging. Its better to represent moves as integers. It would be faster to generate and consume way less memory.

Talparov is still under development, so feel free to fork the repo and work on it.

**Compatible UCI GUIs:**
1.  [Arena](http://www.playwitharena.de/)
2.  [Scid vs. PC](https://scidvspc.sourceforge.net/)

Talparov waits for the `go ponder` command if pondering is set to true. Scid vs. PC doesn't tell the engines to ponder. While Arena does, so pondering works only with Arena.


I'm not a computer chess master, the whole work is done by reading about the concepts and implementing them. [Computer Chess Wiki](https://www.chessprogramming.org/Main_Page) is more than enough to guide you through the world of computer chess.

A lot of open-source engines are available online, I've peeked into Toga and Stockfish to know how to get things done in the best way.
