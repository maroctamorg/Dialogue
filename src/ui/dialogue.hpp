#ifndef TEST_MENU_HPP
#define TEST_MENU_HPP

#include "Tsukuyomi/ui/Tsukuyomi.hpp"


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

class Chat final : public UI_Element {
private:
    std::string font;
    int ptsize;
    SDL_Color font_color {0, 0, 0, 255};
    
    std::shared_ptr<Layout> main_layout;
    std::shared_ptr<Layout> output_layout;
    std::shared_ptr<Layout> response_layout;

    std::vector<std::shared_ptr<TextBox>> output_text;
    std::vector<std::shared_ptr<TextBox>> responses;

public:
    Chat(const std::shared_ptr<GraphicsContext> context, const std::string font, int ptsize, const SDL_Colour font_color, ALIGN_X alignX = ALIGN_X::CENTER, ALIGN_Y alignY = ALIGN_Y::CENTER, SDL_Rect rect = SDL_Rect({0, 0, 0, 0}), SDL_Colour color = SDL_Colour({0, 0, 0, 0}), SDL_Colour border_color = SDL_Colour({0, 0, 0, 0}), bool hidden = false, int r = 0)
    :   UI_Element(context, rect, color, border_color, hidden, r), font(font), ptsize(ptsize), font_color(font_color), align_x{alignX}, align_y{alignY} {
        
    }

    ~Chat() = default;

private:
    void updateLayout();

public:
    void writeOutput(char content, int ptsize = this->ptsize, SDL_Color font_color = this->font_color) {
        // register animations to animation handler?
        // or have the chat handle its built-in animations via its render function and an animation object (better control)?
        // or perhaps game's animation handler registers animation to animation handler which then writes letter by letter to the chat object,
        // which upon finish flushes into a new textbox object; everytime a \n is encountered, the textbox is completed and the layout adds a new container
        // below; once a textbox reaches the bottom, all containers are pushed up (extra animation) just enough for the new textbox
        // (need to have knowledge of how large text is); add scroll up feature

        // need to be able to add containers dynamically to a layout...

        if(output_text.empty()) {
            // output_layout = std::make
        }
    }

    void writeResponses(std::vector<std::string> reponses, Input& input, int ptsize = this->ptsize, SDL_Color font_color = this->font_color) {
        // register callbacks for responses objects: send selection (i.e. index+1) to input object if input is reading?
        // or not, have the input object query the chat object as to whether the button objects were pressed?
    }

public:
    void render() override;
    void updatePosition (const SDL_Rect& rect) override;
    void updateSize() override;

};


class Dialogue_UI final {
private:
    std::shared_ptr<Menu> menu;
    
    // main layout + settings buttons (quit, pause...)
    std::shared_ptr<Layout> main_layout;

    // dynamic text output (self-updating text output layout)
    // reponses
    std::shared_ptr<Chat> chat;
    
public:
    Input input;
    Animation_Handler animation_handler;

private:
    const void construct(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler);

public:
    void writeOutput();
    void writeInput();
    void display();

public:
    Dialogue();
    Dialogue~() = default;
}

#endif