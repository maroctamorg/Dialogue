#include "parser.hpp"

bool Parser::getStringUpToChar(std::stringstream& stream, std::string& sentence, char delim) {
    // std::cout << "string being parsed " <<  stream.str() << " up to character: " << delim << '\n';
    std::string buffer;
    char buffer_c;
    char previous {NULL};
    sentence.clear();
    // stream.seekg(stream.beg); // can't do this... stream object needs to progress as expected by the remaining execution of the program
    stream >> buffer_c;
    if(buffer_c != delim)
        stream.seekg(stream.tellg().operator-(1));
    else {
        return true;
    }
    do {
        if(previous) {
            sentence += delim;
            // stream.seekg(stream.tellg() + 1);
        }
        std::getline(stream, buffer, delim);
        sentence.append(buffer);
        // std::cout << "sentence iteration: " << sentence << '\n';
        if(stream.eof()) break;
        stream.seekg(stream.tellg().operator-(2));
        stream >> previous;
        // std::cout << "previous: " << previous << '\n';
        stream.seekg(stream.tellg().operator+(1));
        // std::cout << "currentpos: " << stream.tellg() << '\n';
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
        if(stream.eof()) break;
        stream >> next;
        switch(next) {
            case('n'): {
                sentence += '\n';
                break;
            }
            case('\\'): {
                sentence += '\\';
                // stream.seekg(stream.tellg() + 1);
            }
            default: {
                sentence += next;
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
    // std::cout << "Parsing unique id: " << number_s.str() << " for word: " << word << '\n';
    long id { stol(number_s.str()) };
    return id;
}

bool Parser::goTo(int argument) {
    auto pos = script.tellg();
    std::string buffer;
    int test;
    script.seekg(script.beg);

    do {
        getStringUpToChar(script, buffer, '#');
        // std::getline(script, buffer); // flushes newline (not needed anymore)
        script >> test;
    } while(test < argument);

    if(test == argument) {
        script.seekg(script.tellg().operator-(1));
        return true;
    }
    
    script.seekg(pos);
    return false;
}

void Parser::setSelection(int value) {
    selection = value;
}

void Parser::loadScene(std::string scene) {
    // std::cout << "Loading scene " << scene << '\n';
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
bool Parser::execute(std::string sentence_s) {
    if(sentence_s.empty()) return true;
    std::stringstream sentence {sentence_s};
    std::string buffer;
    int line;
    std::string verb;
    sentence >> line;
    sentence >> verb;
    // std::cout << "Sentence being parsed: " << sentence.str() << '\n';
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
            // std::getline(script, buffer); //flushes the script stream to the next line
            return true;
            break;
        }
        case RES: {
            std::vector<std::string> options;
            std::string option;
            getStringUpToChar(sentence, buffer, '\'');
            // std::cout << "Entering loop...\n";
            while(getStringUpToChar(sentence, option, '\'')) {
                if(option.empty() || option == "f this?") break;
                // std::cout << "option:" << option << '\n';
                // std::cout << "sentence position: " << sentence.tellg() << '\n';
                options.push_back(option);
                getStringUpToChar(sentence, buffer, '\'');
                option.clear();
            }
            // std::cout << "Exiting loop...\n";
            std::for_each(options.begin(), options.end(), parseEscapedChar); // what is passed to the function: the iterator or the content it is pointing to?
            game.writeAnswers(options);
            // std::getline(script, buffer); //flushes the script stream to the next line
            return true;
            break;
        };
        case IF: {
            int argument;
            sentence >> argument;
            // std::cout << "Checking selection (" << selection << ") against argument (" << argument << ")\n";
            if(selection == argument) {
                selection = 0;
                std::string subsentence;
                // sentence.seekg(sentence.tellg()+1); // not needed, the stream >> already ignores whitespaces + is quite ugly and inflexible
                getStringUpToChar(sentence, subsentence, '#');
                std::stringstream conditional;
                conditional << argument << " " << subsentence << '\n';
                return this->execute(conditional.str());
            }
            // std::getline(script, buffer); //flushes the script stream to the next line
            return true;
            break;
        }
        case GOTO: {
            int argument;
            sentence >> argument;
            if(!goTo(argument)) return false;
            // doesn't work since we don't know the line (here line is the selection...) 
            // for (int i = 0; i < argument - line; i++) {
            //     script.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            // }
            return true;
            break;
        }
        case LOAD: {
            std::string scene;
            getStringUpToChar(sentence, buffer, '\'');
            getStringUpToChar(sentence, scene, '\'');
            parseEscapedChar(scene);
            loadScene(scene);
            return true;
            break;
        }
        default: {
            // std::getline(script, buffer); //flushes the script stream to the next line
            return true;
            break;
        }
    }
}

bool Parser::execute() {
    // std::cout << "Call to parser execute\n";
    std::string sentence;
    getStringUpToChar(script, sentence, '#');
    // std::cout << "\nScript: " << script.str() << "\n\n";
    // std::cout << "Sentence being parsed: " << sentence << '\n';
    done = !this->execute(sentence);
    // std::getline(script, sentence); //flushes the script stream to the next line
    // std::cout << "Exiting parser execute\n";
    return done;
}

Parser::Parser(Game& game)
    : game {game} {}