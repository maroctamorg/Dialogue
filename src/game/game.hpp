#ifndef GAME_HPP
#define GAME_HPP

#include "../dependencies/external.hpp"
#include "../ui/dialogue.hpp"
#include "parser.hpp"

/*
    This seems to be a more general problem afflicting my code: as I pursue class abstraction and specialisation, it seems that the overhead becomes
    unacceptable (most functions have barely a few lines, so calling them is basically like a Matryoshka, quite wasteful).
    Consider taking a more systems/horizontal approach to coding
*/

class Write_Animation : public Animation {
private:
    uint index {0};
    std::string content;
    uint frequency;

public:
    void next() override;

public:
    Write_Animation(std::string content, uint frequency, bool blocking = true, bool deleteWhenDone = true)
        : Animation(blocking, deleteWhenDone), content {content}, frequency {frequency} {}

    ~Write_Animation() override {}
};


class Game final {
public:
    Input input;
    Animation_Handler animation_handler;
    Dialogue_UI ui; //private?
    Parser parser;

public:
    void write(std::string content); // registers blocking animation (type one character at a time with animation handler)
    void writeAnswers(std::initializer_list<std::string> answers);
    void writeAnswers(std::vector<std::string> answers);
    void display();
    void run();

public:
    Game(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler);
};

#endif