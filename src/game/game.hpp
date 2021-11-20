#ifndef GAME_HPP
#define GAME_HPP

#include "../dependencies/external.hpp"

/*
    This seems to be a more general problem afflicting my code: as I pursue class abstraction and specialisation, it seems that the overhead becomes
    unacceptable (most functions have barely a few lines, so calling them is basically like a Matryoshka, quite wasteful).
    Consider taking a more systems/horizontal approach to coding
*/

enum class EVENT_TYPES {
    NO_EVENT = -1,
    SELECTION = 0,
};

typedef struct {
    EVENT_TYPES type { EVENT_TYPES::NO_EVENT};
    int selection { 0 };
} Event;

class Input final {
private:
    Event event;
    uint selected {0};
    uint max;
    bool control {false};

public:
    // Event& poll();
    int poll();
    void listen(uint max);
    int read();
    bool pending();
    void free();
    // bool isSelected(uint selection);

public:
    Input() = default;

};

class Animation {
protected:
    uint counter {0};
    bool active {false};
    bool done {false};

public:
    const bool blocking {false};
    const bool deleteWhenDone {false};

public:
    bool isActive() const;
    bool isDone() const;
    void start();
    virtual void next();
    void end();
    void reset();

public:
    Animation(bool blocking, bool deleteWhenDone)
        : blocking {blocking}, deleteWhenDone {deleteWhenDone} {}

    virtual ~Animation() = default;
};

// Implement queue for animations that cannot run simultaneously to other animations (all blocking animations?)
// run first queued animations in order, then free animations simultaneously; or always have the free animations running
// and only run queued animations in order in parallel (probably this is the best idea)

class Animation_Handler final {
private:
    uint counter {0};
    std::map<uint, std::unique_ptr<Animation>> animations;

public:
    uint add(std::unique_ptr<Animation> animation);
    void remove(uint animation);
    void start(uint animation);
    bool pending() const; //blocking animations running?
    void update();

public:
    Animation_Handler() = default;
};

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

public:
    void write(std::string content); // registers blocking animation (type one character at a time with animation handler)
    void writeAnswers(std::initializer_list<std::string> answers);
    void writeAnswers(std::vector<std::string> answers);
    void display();

public:
    Game() = default;
};

#endif