#ifndef PARSER_HPP
#define PARSER_HPP

#include "../dependencies/external.hpp"
#include "game.hpp"

class Parser {
private:


private:
    const std::map<char, uint> alphabet {
        {' ', 0},
        {'a', 1},
        {'b', 2},
        {'c', 3},
        {'d', 4},
        {'e', 5},
        {'f', 6},
        {'g', 7},
        {'h', 8},
        {'i', 9},
        {'j', 10},
        {'k', 11},
        {'l', 12},
        {'m', 13},
        {'n', 14},
        {'o', 15},
        {'p', 16},
        {'q', 17},
        {'r', 18},
        {'s', 19},
        {'t', 20},
        {'u', 21},
        {'v', 22},
        {'w', 23},
        {'x', 24},
        {'y', 25},
        {'z', 26},
        {'-', 27},
        {',', 28},
        {'.', 29}
    };

    long uniqueId(std::string word);

    enum verb {
        SAY  = 19125,
        RES  = 18519,
        GOTO = 7152015,
        IF   = 96,
        LOAD = 121514,
        END  = 5144
    };

private:
    Game& game;
    std::stringstream script;
    int selection {0};

public:
    void setSelection(int value);
    void loadScene(std::string scene); //maybe use spaces for parsing for now...
    // write contents of script into stringstream and remove all tabs and spaces that are not inside single quotes
    bool execute(std::string sentence);
    bool execute(std::stringstream sentence);
    bool execute();

public:
    Parser(Game& game);
    ~Parser() = default;
};

#endif