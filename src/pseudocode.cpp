#include "unistd.h"

#include "dependencies/external.hpp"
#include "game/parser.hpp"
#include "game/game.hpp"

bool execute(Game& game) {
    static uint counter {0};
    bool control {false};
    switch (counter) {
        case 0:
            game.write("Welcome to the game handler class test.\nThe parser is not yet implemented, so I am simulating the succesive instructions which would be read from a script.\n");
            break;

        case 1:
            game.writeAnswers( {"Ok.", "What is the point to all of this?", "Boring...", "Cool!"} );
            break;

        case 2:
            game.write("Goodbye!\n");
            break;

        case 3:
            control = true;
            break;
        default:
            break;
    }
    counter++; 
    return control;
}

int main() {
    // Event event;
    Game game {};
    // Parser parser(game);
    // parser.loadScene("test.scene");
    int selection {0};
    bool done {false};

    std::cout << "\033[2J";

    game.run();

    // while(!done) {
    //     // event = game.input.poll();
    //     // if(game.input.poll() < 0) {//handles input such as pause (go to menu), quit and other static functionality built-in directly to game_handler
    //     //     done = true;
    //     //     break;
    //     // }
    //     // game.display(); //renders and displays to screen: separate rendering and displaying?
    //     game.animation_handler.update();
    //     if(game.animation_handler.pending()) {
    //         // std::cout << "animation blocking...\n";
    //         continue;
    //     } else if (game.input.pending()) {
    //         // std::cout << "input blocking...\n";
    //         selection = game.input.read(); //acts on custom input previously polled: e.g. select option (writes it to current selection buffer) + sets input from pending to non-pending
    //         if (selection < 0) {
    //             done = true;
    //         } else if (selection > 0) {
    //             parser.setSelection(selection);
    //         }
    //         continue;
    //     } else {
    //         // std::cout << "executing game...\n";
    //         // done = execute(game);
    //         done = parser.execute();
    //     }
    // }

    return 0;
}