#ifndef PARSER_HPP
#define PARSER_HPP

#include "../dependencies/external.hpp"
#include "game.hpp"

class Parser {
private:


private:
    static bool getStringUpToChar(std::stringstream& stream, std::string& sentence, char delim);
    static void parseEscapedChar(std::string& sentence);

    static const std::map<char, uint> alphabet;

    static long uniqueId(std::string word);

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