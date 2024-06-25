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

int main() {
    // Load variables
    loadVariables("variables.txt");

    // Load logic
    std::vector<std::string> logic;
    loadLogic("logic2.txt", logic);

    // Print variables before execution
    std::cout << "-------" << "Variables before execution:" << "-------" << std::endl;
    printVariables(variableMap);
    std::cout << "-------" << "-------" << std::endl;

    // Parse and execute logic
    LadderLogicParser parser(logic, variableMap);
    parser.parseAndExecute();

    // Print variables after execution
    std::cout << "-------" << "Variables after execution:" << "-------" << std::endl;
    printVariables(variableMap);
    std::cout << "-------" << "-------" << std::endl;

    // Save variables
    saveVariables("variables.txt", variableMap);

    return 0;
}
