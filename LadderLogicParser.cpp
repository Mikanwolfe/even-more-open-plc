#include "LadderLogicParser.h"
#include <iostream>
#include <sstream>

LadderLogicParser::LadderLogicParser(const std::vector<std::string>& logic, std::map<std::string, Variable>& variableMap)
    : logic(logic), variableMap(variableMap) {}

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
        while (iss >> instruction) {
            handleInstruction(instruction);
        }
    }
}

void LadderLogicParser::handleInstruction(const std::string& instruction) {
    std::string opcode = instruction.substr(0, 3);
    std::string params = instruction.substr(4, instruction.length() - 5);

    if (opcode == "XIC") {
        bool value = getBoolValue(params);
        std::cout << "XIC(" << params << ") = " << value << std::endl;
    } else if (opcode == "XIO") {
        bool value = !getBoolValue(params);
        std::cout << "XIO(" << params << ") = " << value << std::endl;
    } else if (opcode == "OTE") {
        bool xicValue = getBoolValue("XIC"); // This should be adjusted to the actual logic
        bool xioValue = getBoolValue("XIO"); // This should be adjusted to the actual logic
        bool result = xicValue && xioValue;
        setBoolValue(params, result);
        std::cout << "OTE(" << params << ") = " << result << std::endl;
    } else if (opcode == "ADD") {
        // Split parameters for ADD
        std::istringstream paramStream(params);
        std::string var1, var2, var3;
        std::getline(paramStream, var1, ',');
        std::getline(paramStream, var2, ',');
        std::getline(paramStream, var3, ',');

        double val1 = std::holds_alternative<int>(variableMap[var1]) ? std::get<int>(variableMap[var1]) : std::get<double>(variableMap[var1]);
        double val2 = std::holds_alternative<int>(variableMap[var2]) ? std::get<int>(variableMap[var2]) : std::get<double>(variableMap[var2]);
        variableMap[var3] = val1 + val2;
        std::cout << "ADD(" << var1 << "," << var2 << "," << var3 << ") = " << val1 + val2 << std::endl;
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
