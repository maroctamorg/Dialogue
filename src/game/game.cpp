#include "game.hpp"

void Write_Animation::next() {
    // std::cout << "call to write_animation::next\n";
    if(counter != frequency) {
        counter++;
        return;
    } else if(index < content.length()) {
        std::cout << "\033[34;1;1m" << content.at(index) << "\033[0m"<< std::flush;
        index++;
        counter = 0;
        return;
    }

    this->end();
    return;
}

void Game::write(std::string content) {
    // std::cout << "call to write...\n";
    std::unique_ptr<Write_Animation> animation { new Write_Animation(content, 200000) };
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

void Game::run() {
    bool done {false};
    while(!done) {
        // event = input.poll();
        // if(input.poll() < 0) {//handles input such as pause (go to menu), quit and other static functionality built-in directly to game_handler
        //     done = true;
        //     break;
        // }
        // display(); //renders and displays to screen: separate rendering and displaying?
        animation_handler.update();
        if(animation_handler.pending()) {
            // std::cout << "animation blocking...\n";
            continue;
        } else if (input.pending()) {
            // std::cout << "input blocking...\n";
            selection = input.read(); //acts on custom input previously polled: e.g. select option (writes it to current selection buffer) + sets input from pending to non-pending
            if (selection < 0) {
                done = true;
            } else if (selection > 0) {
                parser.setSelection(selection);
            }
            continue;
        } else {
            // std::cout << "executing game...\n";
            // done = execute(game);
            done = parser.execute();
        }
    }
}

Game::Game(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler)
    :   parser(this) {
        parser.loadScene("init.scene");
        ui.construct(std::shared_ptr<GraphicsContext> context, std::shared_ptr<EventHandler> handler);
    }