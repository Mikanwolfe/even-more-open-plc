#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>
#include "LadderLogicInterpreter.h"

void readLadderLogicFromFile(const std::string &filename, std::string &ladderLogic) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    ladderLogic = buffer.str();
    std::cout << "Loaded ladder logic from file: " << filename << std::endl;
}

int main(int argc, char *argv[]) {
    std::string filename;
    int numScans = 10; // Default number of scans

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-n" && i + 1 < argc) {
            numScans = std::stoi(argv[++i]);
        } else {
            filename = argv[i];
        }
    }

    if (filename.empty()) {
        std::cerr << "Usage: " << argv[0] << " <filename> [-n numScans]" << std::endl;
        return EXIT_FAILURE;
    }

    std::string ladderLogic;

    // Read ladder logic from file
    readLadderLogicFromFile(filename, ladderLogic);

    LadderLogicInterpreter interpreter;

    // Load variables from file
    interpreter.loadVariablesFromFile("variables.txt");

    // Parse the ladder logic
    interpreter.parseLadderLogic(ladderLogic);

    // Execute the ladder logic for the specified number of scans
    for (int scan = 0; scan < numScans; ++scan) {
        std::cout << "Scan " << (scan + 1) << std::endl;
        interpreter.executeLadderLogic();
        interpreter.printRelayStatus();
        interpreter.printVariableStatus();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Save variables to file
    interpreter.saveVariablesToFile("variables.txt");

    return 0;
}
