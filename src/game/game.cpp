#include "game.hpp"

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
        std::cout << "You've entered '" << digit << "'\n"; 
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

void Write_Animation::next() {
    // std::cout << "call to write_animation::next\n";
    if(counter != frequency) {
        counter++;
        return;
    } else if(index < content.length()) {
        std::cout << "\033[31;1;4m" << content.at(index) << "\033[0m"<< std::flush;
        index++;
        counter = 0;
        return;
    }

    this->end();
    return;
}

void Game::write(std::string content) {
    // std::cout << "call to write...\n";
    std::unique_ptr<Write_Animation> animation { new Write_Animation(content, 100000) };
    uint id { animation_handler.add(std::move(animation)) };
    animation_handler.start(id);
}

void Game::writeAnswers(std::initializer_list<std::string> answers) {
    // std::cout << "Call to write answers..\n";
    std::initializer_list<std::string>::iterator it {answers.begin()};
    uint counter {1};
    std::stringstream content;
    while(it != answers.end()) {
        content << counter << ". " << (*it) << '\n';
        counter++;
        it++;
    }

    this->write(content.str());

    // tell input to listen for response (blocking mode)
    input.listen(answers.size());
}

void Game::writeAnswers(std::vector<std::string> answers) {
    // std::cout << "Call to write answers..\n";
    std::vector<std::string>::iterator it {answers.begin()};
    uint counter {1};
    std::stringstream content;
    while(it != answers.end()) {
        content << counter << ". " << (*it) << '\n';
        counter++;
        it++;
    }

    this->write(content.str());

    // tell input to listen for response (blocking mode)
    input.listen(answers.size());
}

void Game::display() {

};