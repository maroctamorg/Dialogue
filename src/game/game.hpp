#include ../dependencies/external.hpp

class Game {
protected:
    UI ui;
    Inventory inventory;
    Information information;
    int progress;
    Map map;

public:
    Game(UI ui, Inventory inventory, Information information, Map map)
        :   ui(ui), inventory(inventory), information(information), map(map)    {}
    ~Game() = default;

public:
    void parse(const std::string &input) {
        input.split(' ');
        switch(input[0]) {
            case
        }
    }

    void update() {
        Input input;
        switch(ui.update(&input)) {
            default:
                break;
        }
    }

};