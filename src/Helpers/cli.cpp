#include <iostream>
#include "cli.h"

using namespace std;

Cli::Cli(int argc, char **argv) {
    this->argc = argc;
    this->argv = argv;
}

string Cli::getArgument(string name) {
    for (int i = 0; i < argc; i++) {
        string input = argv[i];
        unsigned long split;
        if (input.length() > 2 && input.substr(0, 2) == "--" && (split = input.find("=")) > 0) {
            string key = input.substr(2, split - 2);
            string value = input.substr(split + 1);
            if (key == name) {
                return value;
            }
        }
    }

    return "";
}

int Cli::getInt(string key, int defaultValue) {
    string value = getArgument(key);
    if (value != "") {
        try {
            return stoi(value);
        } catch (const std::invalid_argument& ia) {
            std::cerr << "[ERROR] For argument \"" << key << "\" value \"" << value << "\" is not a number."
                      << " Using default: " << defaultValue << ". Error details: " << ia.what() << endl;
        }
    }
    return defaultValue;
}