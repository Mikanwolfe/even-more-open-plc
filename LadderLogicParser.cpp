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

    if (instruction.length() < 3) {
        std::cerr << "Invalid instruction: " << instruction << std::endl;
        return;
    }

    std::string opcode = instruction.substr(0, 3);
    std::string params = (instruction.length() > 3) ? instruction.substr(4, instruction.length() - 5) : "";

    if (opcode == "END") {
        endFound = true;
        std::cout << "End found, stopping further instructions." << std::endl;
    } else if (opcode == "XIC") {
        if (params.empty()) {
            std::cerr << "XIC instruction missing parameters." << std::endl;
            return;
        }
        bool value = getBoolValue(params);
        lineState = lineState && value;
        std::cout << "XIC(" << params << ") = " << value << ", Line = " << lineState << std::endl;
    } else if (opcode == "XIO") {
        if (params.empty()) {
            std::cerr << "XIO instruction missing parameters." << std::endl;
            return;
        }
        bool value = !getBoolValue(params);
        lineState = lineState && value;
        std::cout << "XIO(" << params << ") = " << value << ", Line = " << lineState << std::endl;
    } else if (opcode == "OTE") {
        if (params.empty()) {
            std::cerr << "OTE instruction missing parameters." << std::endl;
            return;
        }
        setBoolValue(params, lineState);
        std::cout << "OTE(" << params << ") = " << lineState << std::endl;
    } else if (opcode == "ADD" || opcode == "SUB") {
        if (params.empty()) {
            std::cerr << opcode << " instruction missing parameters." << std::endl;
            return;
        }
        std::istringstream paramStream(params);
        std::string var1, var2, var3;
        std::getline(paramStream, var1, ',');
        std::getline(paramStream, var2, ',');
        std::getline(paramStream, var3, ',');

        if (var1.empty() || var2.empty() || var3.empty()) {
            std::cerr << opcode << " instruction has incomplete parameters." << std::endl;
            return;
        }

        try {
            if (opcode == "ADD") {
                handleAddInstruction(var1, var2, var3);
            } else if (opcode == "SUB") {
                handleSubInstruction(var1, var2, var3);
            }
        } catch (const std::exception& e) {
            std::cerr << opcode << " instruction error: " << e.what() << std::endl;
        }
    } else {
        std::cerr << "Unknown instruction: " << instruction << std::endl;
    }
}

void LadderLogicParser::handleAddInstruction(const std::string& var1, const std::string& var2, const std::string& var3) {
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
}

void LadderLogicParser::handleSubInstruction(const std::string& var1, const std::string& var2, const std::string& var3) {
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
}


bool LadderLogicParser::getBoolValue(const std::string& varName) {
    auto it = variableMap.find(varName);
    if (it != variableMap.end()) {
        if (std::holds_alternative<bool>(it->second)) {
            return std::get<bool>(it->second);
        } else {
            throw std::invalid_argument("Variable " + varName + " is not of type bool.");
        }
    }
    throw std::invalid_argument("Variable " + varName + " not found.");
}

void LadderLogicParser::setBoolValue(const std::string& varName, bool value) {
    variableMap[varName] = value;
}
