#include "parser.hpp"

bool Parser::getStringUpToChar(std::stringstream& stream, std::string& sentence, char delim) {
    std::string buffer;
    char previous {NULL};
    sentence.clear();
    do {
        if(previous) {
            sentence += delim;
            stream.seekg(stream.tellg() + 1);
        }
        std::getline(stream, buffer, delim);
        sentence.append(buffer);
        stream.seekg(stream.tellg() - 2);
        stream >> previous;
    } while(previous == '\\');

    return !sentence.empty();
}

void Parser::parseEscapedChar(std::string& sentence) {
    std::stringstream stream {sentence};
    std::string buffer;
    char next;
    sentence.clear();
    while (std::getline(stream, buffer, '\\')) {
        sentence.append(buffer);
        stream >> next;
        switch(next) {
            case('n'): {
                sentence += '\n';
                break;
            }
            case('\\'): {
                sentence += '\\';
                stream.seekg(stream.tellg() + 1);
            }
            default: {
                break;
            }
        }
    }
}

const std::map<char, uint> Parser::alphabet {
    {' ', 0},   {'a', 1},   {'b', 2},   {'c', 3},   {'d', 4},   {'e', 5},
    {'f', 6},   {'g', 7},   {'h', 8},   {'i', 9},   {'j', 10},  {'k', 11},
    {'l', 12},  {'m', 13},  {'n', 14},  {'o', 15},  {'p', 16},  {'q', 17},
    {'r', 18},  {'s', 19},  {'t', 20},  {'u', 21},  {'v', 22},  {'w', 23},
    {'x', 24},  {'y', 25},  {'z', 26},  {'-', 27},  {',', 28},  {'.', 29}
};

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
            getStringUpToChar(sentence, buffer, '\'');
            getStringUpToChar(sentence, content, '\'');
            parseEscapedChar(content);
            game.write(content);
            return true;
            break;
        }
        case RES: {
            std::vector<std::string> options;
            std::string option;
            getStringUpToChar(sentence, buffer, '\'');
            std::cout << "Entering loop...\n";
            while(getStringUpToChar(sentence, option, '\'')) {
                if(option.empty() || option == "f this?") break;
                std::cout << "option:" << option << '\n';
                std::cout << "sentence position: " << sentence.tellg() << '\n';
                options.push_back(option);
                getStringUpToChar(sentence, buffer, '\'');
                option.clear();
            }
            std::cout << "Exiting loop...\n";
            std::for_each(options.begin(), options.end(), parseEscapedChar); // what is passed to the function: the iterator or the content it is pointing to?
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
                // sentence.seekg(sentence.tellg()+1); // not needed, the stream >> already ignores whitespaces + is quite ugly and inflexible
                getStringUpToChar(sentence, subsentence, '.');
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
            getStringUpToChar(sentence, buffer, '\'');
            getStringUpToChar(sentence, scene, '\'');
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
    getStringUpToChar(script, sentence, '.');
    // std::cout << "\nScript: " << script.str() << "\n\n";
    // std::cout << "Sentence being parsed: " << sentence << '\n';
    this->execute(std::stringstream(sentence));
    std::getline(script, sentence); //flushes the script stream to the next line
    // std::cout << "Exiting parser execute\n";
}

Parser::Parser(Game& game)
    : game {game} {}