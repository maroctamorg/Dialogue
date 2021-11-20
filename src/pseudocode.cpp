#include "unistd.h"

#include "dependencies/external.hpp"
// #include "game/parser.hpp"
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
    // Parser parser;
    // parser.loadScene("start_scene");
    uint selection {0};
    bool done {false};
    while(!done) {
        // event = game.input.poll();
        // if(game.input.poll() < 0) {//handles input such as pause (go to menu), quit and other static functionality built-in directly to game_handler
        //     done = true;
        //     break;
        // }
        game.display(); //renders and displays to screen: separate rendering and displaying?
        game.animation_handler.update();
        // std::cout << "returned from update...\n";
        if(game.animation_handler.pending()) {
            // std::cout << "animation blocking...\n";
            // sleep(2);
            // continue;
        } else if (game.input.pending()) {
            // std::cout << "input blocking...\n";
            selection = game.input.read(); //acts on custom input previously polled: e.g. select option (writes it to current selection buffer) + sets input from pending to non-pending
            if (selection < 0) done = true;
            continue;
        } else {
            // std::cout << "executing game...\n";
            done = execute(game);

        }

        // std::cout << "returned from execution...\n";
        // parser keeps internal track of which line of the scence script to read untill a scene is exited, when parser.clear() is called
        // parser.execute(selection); // parser executes first line of script (e.g. say 'Welcome to the game' -> calls write function of game which starts a flow interrupting animation)
        /*typical script flow:
            #: script1
            0: say '...' // calls game_handler write function
            1: responses '...', '...', '...' ... // calls game_handler response function: writes to screen ('\n' separated string) + interrupts parser execution until input (sets input to pending) + remembers input
            2: if 1 : goto n // checks if input selected was 1 with game.input.isSelected(1), if so jump execution to line n in script
            3: if 2 : play sound1; goto m
            4: if 3 : load 'script2' // calls game_handler loadScene('script2') function and clears parser
        */
        // do script lines need to be ordered? should parser just know where it is, then execute next line unless told to identify line starting with n?
        // pros: less of a headache when writing the script; cons: messy?
    }

    return 0;
}