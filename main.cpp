#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <variant>
#include <vector>
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

int main() {
    // Load variables
    loadVariables("variables.txt");

    // Load logic
    std::vector<std::string> logic;
    loadLogic("logic.txt", logic);

    // Parse and execute logic
    LadderLogicParser parser(logic, variableMap);
    parser.parseAndExecute();

    return 0;
}
