#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <variant>
#include <vector>
#include <thread>
#include <chrono>
#include "LadderLogicParser.h"

// Define the variant type for variables
using Variable = std::variant<int, bool, double>;
std::map<std::string, Variable> variableMap;

// Function to load variables from a file
void loadVariables(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string name, type;
        iss >> name >> type;
        if (type == "int") {
            int value;
            iss >> value;
            variableMap[name] = value;
        } else if (type == "bool") {
            bool value;
            iss >> value;
            variableMap[name] = value;
        } else if (type == "real") {
            double value;
            iss >> value;
            variableMap[name] = value;
        }
    }
}

// Function to save variables to a file
void saveVariables(const std::string& filename, const std::map<std::string, Variable>& variableMap) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Failed to open " << filename << std::endl;
        return;
    }
    for (const auto& [name, value] : variableMap) {
        file << name << " ";
        if (std::holds_alternative<int>(value)) {
            file << "int " << std::get<int>(value) << "\n";
        } else if (std::holds_alternative<bool>(value)) {
            file << "bool " << std::get<bool>(value) << "\n";
        } else if (std::holds_alternative<double>(value)) {
            file << "real " << std::get<double>(value) << "\n";
        }
    }
}

std::string toString(bool value) {
    return value ? "true" : "false";
}

// Function to load logic from a file
void loadLogic(const std::string& filename, std::vector<std::string>& logic) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        logic.push_back(line);
    }
}

void printVariables(const std::map<std::string, Variable>& variableMap) {
    for (const auto& [name, value] : variableMap) {
        std::cout << name << " = ";
        if (std::holds_alternative<int>(value)) {
            std::cout << std::get<int>(value);
        } else if (std::holds_alternative<bool>(value)) {
            std::cout << toString(std::get<bool>(value));
        } else if (std::holds_alternative<double>(value)) {
            std::cout << std::get<double>(value);
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::string logicFile = "logic4.txt";
    bool testMode = false;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-f" && i + 1 < argc) {
            logicFile = argv[++i];
        }
        
        if (std::string(argv[i]) == "-t") {
            testMode = true;
        }
    }

    // Load variables
    loadVariables("variables.txt");

    // Load logic
    std::vector<std::string> logic;
    loadLogic(logicFile, logic);

    // Initialize the parser once
    LadderLogicParser parser(logic, variableMap);

    if (testMode) {
        // Keep scanning with a delay of 10ms
        while (true) {
            // Print variables before execution
            std::cout << "-------" << "Variables before execution:" << "-------" << std::endl;
            printVariables(variableMap);
            std::cout << "-------" << "-------" << std::endl;

            // Execute logic without re-initializing the parser
            parser.executeLogic();

            // Print variables after execution
            std::cout << "-------" << "Variables after execution:" << "-------" << std::endl;
            printVariables(variableMap);
            std::cout << "Scan time: " << parser.scanTime << " us" << std::endl;
            std::cout << "-------" << "-------" << std::endl;

            // Save variables
            // saveVariables("variables.txt", variableMap); //Standly with the new mapping routine this core dumps

            // Delay for 10ms
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    } else {
        // Single execution mode

        // Print variables before execution
        std::cout << "-------" << "Variables before execution:" << "-------" << std::endl;
        printVariables(variableMap);
        std::cout << "-------" << "-------" << std::endl;

        // Execute logic without re-initializing the parser
        parser.executeLogic();

        // Print variables after execution
        std::cout << "-------" << "Variables after execution:" << "-------" << std::endl;
        printVariables(variableMap);
        std::cout << "Scan time: " << parser.scanTime << " ms" << std::endl;
        std::cout << "-------" << "-------" << std::endl;

        // Save variables
        // saveVariables("variables.txt", variableMap);
    }

    return 0;
}
