#include "parser.hpp"

long Parser::uniqueId(std::string word) {
    std::stringstream number_s;
    for(auto it = word.cbegin(); it < word.cend(); it++) {
        number_s << alphabet.at(*it);
    }
    std::cout << "Parsing unique id: " << number_s.str() << " for word: " << word << '\n';
    long id { stol(number_s.str()) };
    return id;
}

void Parser::setSelection(int value) {
    selection = value;
}

void Parser::loadScene(std::string scene) {
    // std::cout << "Call to parser loadScene\n";
    std::ifstream file("src/scenes/" + scene);
    script.str("");
    script.clear();
    script << file.rdbuf();
    file.close();
    // std::cout << "Exiting parser loadScene\n";
}

// bool Parser::execute(std::string sentence) {
//     int start_index = sentence.find(' ') + 1;
//     std::string verb { sentence.substr(start_index, sentence.find(' ', start_index) - start_index)};
//     switch(uniqueId(verb)) {
//         case END: {
//             return false;
//         }
//         case SAY: {
//             game.write(command.at(1));
//             return 
//         }
//     }
// }
bool Parser::execute(std::stringstream sentence) {
    std::string buffer;
    int line;
    std::string verb;
    sentence >> line;
    sentence >> verb;
    std::cout << "Sentence being parsed: " << sentence.str() << '\n';
    // std::cout << "Line being parsed: " << line << '\n';
    // std::cout << "verb: " << verb << '\n';

    switch(uniqueId(verb)) {
        case END: {
            script.clear();
            return false;
            break;
        }
        case SAY: {
            std::string content;
            std::getline(sentence, buffer, '\'');
            std::getline(sentence, content, '\'');
            game.write(content);
            return true;
            break;
        }
        case RES: {
            std::vector<std::string> options;
            std::string option;
            std::getline(sentence, buffer, '\'');
            std::cout << "Entering loop...\n";
            while(std::getline(sentence, option, '\'')) {
                if(option.empty() || option == "f this?") break;
                std::cout << "option:" << option << '\n';
                std::cout << "sentence position: " << sentence.tellg() << '\n';
                options.push_back(option);
                std::getline(sentence, buffer, '\'');
                option.clear();
            }
            std::cout << "Exiting loop...\n";
            game.writeAnswers(options);
            return true;
            break;
        };
        case IF: {
            int argument;
            sentence >> argument;
            if(selection == argument) {
                selection = 0;
                std::string subsentence;
                std::getline(sentence, subsentence);
                this->execute(std::stringstream(argument + " " + subsentence));
            }
            return true;
            break;
        }
        case GOTO: {
            int argument;
            sentence >> argument;
            for (int i = 0; i < argument - line; i++) {
                script.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            return true;
            break;
        }
        case LOAD: {
            std::string scene;
            std::getline(sentence, buffer, '\'');
            std::getline(sentence, scene, '\'');
            loadScene(scene);
            return true;
            break;
        }
        default: {
            return true;
            break;
        }
    }
}

bool Parser::execute() {
    // std::cout << "Call to parser execute\n";
    std::string sentence;
    std::getline(script, sentence);
    // std::cout << "\nScript: " << script.str() << "\n\n";
    // std::cout << "Sentence being parsed: " << sentence << '\n';
    this->execute(std::stringstream(sentence));
    // std::cout << "Exiting parser execute\n";
}

Parser::Parser(Game& game)
    : game {game} {}