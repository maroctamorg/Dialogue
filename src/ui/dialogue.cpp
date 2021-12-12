#include "dialogue.hpp"
#include <initializer_list>


// poll should listen asynchronously to keyboard input...
int Input::poll() {
    bool done {false};
    std::string input;

    std::cin >> input;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if(input.at(0) == 'q' || input.compare("quit")) {
        this->free();
        return -1;
    }
}

void Input::listen(uint max) {
    if(max < 0) return;
    this->max = max;
    control = true;
    // std::cout << "\033[33;1;1m" << std::flush;
}

int Input::read() {
    bool done {false};
    std::string input;

    std::cin >> input;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if(input.at(0) == 'q' || input == "quit") {
        this->free();
        return -1;
    }

    if(input.length() == 1 && isdigit(input.at(0))) {
        std::stringstream s {&(input.at(0))};
        int digit {0};
        s >> digit;
        // std::cout << "You've entered '" << digit << "'\n"; 
        if( digit > 0 && digit < max + 1) {
            this->free();
            return digit;
        }
    }

    std::cout << "Invalid input, enter number between 1 and " << max << "\n";
    return 0;
}

bool Input::pending() {
    return control;
}

void Input::free() {
    control = false;
    // std::cout << "\033[0m" << std::flush;
}

bool Animation::isDone() const {
    return done;
}

bool Animation::isActive() const {
    return active;
}

void Animation::start() {
    active = true;
}

void Animation::next() {
    std::cout << "Call to Animation base class next...\n";
    counter++;
}

void Animation::end() {
    // std::cout << "Call to end animation...\n";
    active = false;
    done = true;
}

void Animation::reset() {
    active = false;
    done = false;
}

uint Animation_Handler::add(std::unique_ptr<Animation> animation) {
    counter++;
    animations.insert({counter, std::move(animation)});
    return counter;
}

void Animation_Handler::remove(uint animation) {
    animations.erase(animation);
}

void Animation_Handler::start(uint animation) {
    animations.at(animation)->start();
}

bool Animation_Handler::pending() const {
    bool blocking {false};
    std::map<uint, std::unique_ptr<Animation>>::const_iterator it {animations.cbegin()};
    while(it != animations.cend()) {
        blocking = blocking || (it->second->blocking && it->second->isActive());
        it++;
    }
    return blocking;
}

void Animation_Handler::update() {
    // std::cout << "call to update...\n";
    std::map<uint, std::unique_ptr<Animation>>::iterator it {animations.begin()};
    int del {0};
    while(it != animations.end()) {
        // std::cout << "prepare for doom...\n";
        if(it->second->isActive()) it->second->next();
        else if (it->second->isDone()) {
            if(it->second->deleteWhenDone) del = it->first;
            else it->second->reset();
        }
        it++;
    }

    if(del > 0) animations.erase(del);
}

///////////////////////////////////////////////##########################################################################///////////////////////////////////////

const Dialogue_UI::construct(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler) {
    std::string font = "resources/fonts/CourierNew.ttf";
    SDL_Color font_color { 255, 255, 255, 255 };
    
    // LAYOUTS
    main_layout = std::make_shared<Layout>(context, {Container(0.1, 0.1, 0.8, 0.8)});
    dialogue_layout = std::shared_ptr<Layout>(context, {Container()})

    // BUTTON
    std::shared_ptr<Layout> quit_button_layout {std::make_shared<Layout>( new Layout(context, {Container(0.1, 0.05, 0.8, 0.9)}) )};
    button_layout->placeUI_Element(std::make_shared<TextBox>(context, "Ping Server", font, 15, font_color, ALIGN_X::CENTER, ALIGN_Y::CENTER), 0);
    button = std::make_shared<Button>(context, handler, button_layout, 0, true, false, SDL_Color({172, 43, 12, 125}));
    button->registerCallBack([](const GraphicsContext& context, const EventHandler& handler, Button& button) {
        if(!button.isSelected())
            button.setColor(SDL_Colour({12, 172, 43, 125}));
        else
            button.setColor(SDL_Colour({172, 43, 12, 125}));
    });
    pannel_layout->placeUI_Element(button, 0);

    // pannel_layout->placeUI_Element(new TextBox(context, "Short Text", font, 15, font_color, ALIGN_X::CENTER, ALIGN_Y::CENTER), 0);
    // pannel_layout->placeUI_Element(new TextBox(context, medium, font, 15, font_color, ALIGN_X::CENTER, ALIGN_Y::CENTER), 1);
    
    // INPUT FORM
    form_layout = std::shared_ptr<Layout>( new Layout(context, {Container(0.05, 0.05, 0.2, 0.9), Container(0.35, 0.05, 0.6, 0.9)}) );
    std::shared_ptr<InputField> input_field { std::make_shared<InputField>(context, handler, 0, font, 15, SDL_Color({0, 0, 0, 255})) };
    input_button_layout = std::shared_ptr<Layout>( new Layout(context, {Container(0.1, 0.05, 0.8, 0.9)}) );
    input_button_layout->placeUI_Element(std::make_shared<TextBox>(context, "Submit username:", font, 15, font_color), 0);
    input_button = std::make_shared<Button>(context, handler, input_button_layout, 0, true, false, SDL_Color({172, 43, 12, 125}));
    std::weak_ptr<InputField> field(input_field);
    input_button->registerCallBack([field](const GraphicsContext& context, const EventHandler& handler, Button& button) mutable {
        if(auto input_field = field.lock()) {
            button.setColor(SDL_Colour({12, 172, 43, 125}));
            std::string input = input_field->getText();
            std::cout << "USER INPUT: " << input << '\n';
        }
        else
            std::cout << "Weak pointer could not be resolved!\n";
    });
    form_layout->placeUI_Element(input_button, 0);
    form_layout->placeUI_Element(input_field, 1);
    pannel_layout->placeUI_Element(form_layout, 1);

    text = std::make_shared<TextBox>(context, "Enter a username to join a random session...", font, 15, font_color, ALIGN_X::CENTER, ALIGN_Y::CENTER);
    pannel_layout->placeUI_Element(text, 2);
    main_layout->placeUI_Element(std::make_shared<Pannel>(context, pannel_layout, SDL_Color({24, 43, 100, 255})), 0);
    // main_layout->placeUI_Element(new Pannel(context, nullptr, SDL_Color({24, 43, 100, 255})), 0);
    menu = std::make_shared<Menu>(context, handler, main_layout, SDL_Color({255, 255, 255, 255}));
    return menu;
}

Dialogue::Dialogue() {
    this->construct();
}
