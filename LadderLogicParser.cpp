#include "LadderLogicParser.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

LadderLogicParser::LadderLogicParser(const std::vector<std::string>& logic, std::map<std::string, Variable>& variableMap)
    : logic(logic), variableMap(variableMap), lineState(1), endFound(false) {}

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
    if (endFound) {
        return;
    }

    std::string opcode = instruction.substr(0, 3);

    if (opcode == "END") {
        endFound = true;
        std::cout << "End found, stopping further instructions." << std::endl;
    } else if (opcode == "XIC") {
        std::string params = instruction.substr(4, instruction.length() - 5);
        bool value = getBoolValue(params);
        lineState = lineState && value;
        std::cout << "XIC(" << params << ") = " << value << ", Line = " << lineState << std::endl;
    } else if (opcode == "XIO") {
        std::string params = instruction.substr(4, instruction.length() - 5);
        bool value = !getBoolValue(params);
        lineState = lineState && value;
        std::cout << "XIO(" << params << ") = " << value << ", Line = " << lineState << std::endl;
    } else if (opcode == "OTE") {
        std::string params = instruction.substr(4, instruction.length() - 5);
        setBoolValue(params, lineState);
        std::cout << "OTE(" << params << ") = " << lineState << std::endl;
    } else if (opcode == "ADD") {
        std::string params = instruction.substr(4, instruction.length() - 5);
        std::istringstream paramStream(params);
        std::string var1, var2, var3;
        std::getline(paramStream, var1, ',');
        std::getline(paramStream, var2, ',');
        std::getline(paramStream, var3, ',');

        if (variableMap.find(var1) != variableMap.end() && variableMap.find(var2) != variableMap.end()) {
            if (std::holds_alternative<int>(variableMap[var1]) && std::holds_alternative<int>(variableMap[var2])) {
                int result = std::get<int>(variableMap[var1]) + std::get<int>(variableMap[var2]);
                variableMap[var3] = result;
                std::cout << "ADD(" << var1 << "," << var2 << "," << var3 << ") = " << result << std::endl;
            } else if (std::holds_alternative<double>(variableMap[var1]) && std::holds_alternative<double>(variableMap[var2])) {
                double result = std::get<double>(variableMap[var1]) + std::get<double>(variableMap[var2]);
                variableMap[var3] = result;
                std::cout << "ADD(" << var1 << "," << var2 << "," << var3 << ") = " << result << std::endl;
            } else {
                std::cerr << "ADD instruction type mismatch: " << var1 << ", " << var2 << std::endl;
            }
        } else {
            std::cerr << "ADD instruction variables not found: " << var1 << ", " << var2 << std::endl;
        }
    } else if (opcode == "SUB") {
        std::string params = instruction.substr(4, instruction.length() - 5);
        std::istringstream paramStream(params);
        std::string var1, var2, var3;
        std::getline(paramStream, var1, ',');
        std::getline(paramStream, var2, ',');
        std::getline(paramStream, var3, ',');

        if (variableMap.find(var1) != variableMap.end() && variableMap.find(var2) != variableMap.end()) {
            if (std::holds_alternative<int>(variableMap[var1]) && std::holds_alternative<int>(variableMap[var2])) {
                int result = std::get<int>(variableMap[var1]) - std::get<int>(variableMap[var2]);
                variableMap[var3] = result;
                std::cout << "SUB(" << var1 << "," << var2 << "," << var3 << ") = " << result << std::endl;
            } else if (std::holds_alternative<double>(variableMap[var1]) && std::holds_alternative<double>(variableMap[var2])) {
                double result = std::get<double>(variableMap[var1]) - std::get<double>(variableMap[var2]);
                variableMap[var3] = result;
                std::cout << "SUB(" << var1 << "," << var2 << "," << var3 << ") = " << result << std::endl;
            } else {
                std::cerr << "SUB instruction type mismatch: " << var1 << ", " << var2 << std::endl;
            }
        } else {
            std::cerr << "SUB instruction variables not found: " << var1 << ", " << var2 << std::endl;
        }
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
