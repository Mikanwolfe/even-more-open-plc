#include "LadderLogicParser.h"
#include <iostream>
#include <sstream>

LadderLogicParser::LadderLogicParser(const std::vector<std::string>& logic, std::map<std::string, Variable>& variableMap)
    : logic(logic), variableMap(variableMap), lineState(1) {}

void LadderLogicParser::parseAndExecute() {
    for (const auto& line : logic) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        // Skip lines that do not start with a number
        if (!isdigit(token[0])) {
            continue;
        }

        std::string instruction;
        lineState = 1; // Reset line state for each new line
        while (iss >> instruction) {
            handleInstruction(instruction);
        }
    }
}

void LadderLogicParser::handleInstruction(const std::string& instruction) {
    // Ensure the instruction has a minimum length
    if (instruction.length() < 4) {
        std::cerr << "Invalid instruction: " << instruction << std::endl;
        return;
    }

    std::string opcode = instruction.substr(0, 3);
    std::string params = instruction.substr(4, instruction.length() - 5);

    if (opcode == "XIC") {
        bool value = getBoolValue(params);
        lineState = lineState && value;
        std::cout << "XIC(" << params << ") = " << value << ", Line = " << lineState << std::endl;
    } else if (opcode == "XIO") {
        bool value = !getBoolValue(params);
        lineState = lineState && value;
        std::cout << "XIO(" << params << ") = " << value << ", Line = " << lineState << std::endl;
    } else if (opcode == "OTE") {
        setBoolValue(params, lineState);
        std::cout << "OTE(" << params << ") = " << lineState << std::endl;
    } else {
        std::cerr << "Unknown instruction: " << instruction << std::endl;
    }
}

bool LadderLogicParser::getBoolValue(const std::string& varName) {
    auto it = variableMap.find(varName);
    if (it != variableMap.end() && std::holds_alternative<bool>(it->second)) {
        return std::get<bool>(it->second);
    }
    throw std::invalid_argument("Variable not found or not a bool: " + varName);
}

void LadderLogicParser::setBoolValue(const std::string& varName, bool value) {
    variableMap[varName] = value;
}
