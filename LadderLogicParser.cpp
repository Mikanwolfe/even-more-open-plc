#include "LadderLogicParser.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stack>

LadderLogicParser::LadderLogicParser(const std::vector<std::string>& logic, std::map<std::string, Variable>& variableMap)
    : logic(logic), variableMap(variableMap), lineState(true), endFound(false) {}

std::string boolToString(bool value) {
    return value ? "true" : "false";
}

void LadderLogicParser::parseAndExecute() {
    for (const auto& line : logic) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        // Skip lines that do not start with a number
        if (!isdigit(token[0])) {
            continue;
        }

        std::vector<std::string> tokens;
        while (iss >> token) {
            tokens.push_back(token);
        }

        lineState = true; // Reset line state for each new line
        handleTokens(tokens);
    }
}

void LadderLogicParser::handleTokens(const std::vector<std::string>& tokens) {
    std::stack<bool> branchStack;
    bool branchResult = true;
    bool currentBranchState = true;

    size_t i = 0;
    while (i < tokens.size()) {
        const std::string& instruction = tokens[i];
        std::string opcode = instruction.substr(0, 3);
        std::string params = (instruction.length() > 3) ? instruction.substr(4, instruction.length() - 5) : "";

        try {
            if (opcode == "END") {
                endFound = true;
                std::cout << "End found, stopping further instructions." << std::endl;
                return;
            } else if (opcode == "XIC") {
                if (params.empty()) {
                    std::cerr << "XIC instruction missing parameters." << std::endl;
                    return;
                }
                bool value = getBoolValue(params);
                currentBranchState = currentBranchState && value;
                std::cout << "XIC(" << params << ") = " << boolToString(value) << ", Line = " << boolToString(currentBranchState) << std::endl;
            } else if (opcode == "XIO") {
                if (params.empty()) {
                    std::cerr << "XIO instruction missing parameters." << std::endl;
                    return;
                }
                bool value = !getBoolValue(params);
                currentBranchState = currentBranchState && value;
                std::cout << "XIO(" << params << ") = " << boolToString(value) << ", Line = " << boolToString(currentBranchState) << std::endl;
            } else if (opcode == "OTE") {
                if (params.empty()) {
                    std::cerr << "OTE instruction missing parameters." << std::endl;
                    return;
                }
                setBoolValue(params, currentBranchState);
                std::cout << "OTE(" << params << ") = " << boolToString(currentBranchState) << std::endl;
            } else if (opcode == "ADD") {
                std::istringstream paramStream(params);
                std::string var1, var2, var3;
                std::getline(paramStream, var1, ',');
                std::getline(paramStream, var2, ',');
                std::getline(paramStream, var3, ',');

                if (var1.empty() || var2.empty() || var3.empty()) {
                    std::cerr << "ADD instruction has incomplete parameters." << std::endl;
                    return;
                }

                if (!lineState) {
                    return; // Do not run this instruction if line state is LOW
                }
                handleAddInstruction(var1, var2, var3);
            } else if (opcode == "SUB") {
                std::istringstream paramStream(params);
                std::string var1, var2, var3;
                std::getline(paramStream, var1, ',');
                std::getline(paramStream, var2, ',');
                std::getline(paramStream, var3, ',');

                if (var1.empty() || var2.empty() || var3.empty()) {
                    std::cerr << "SUB instruction has incomplete parameters." << std::endl;
                    return;
                }

                if (!lineState) {
                    return; // Do not run this instruction if line state is LOW
                }

                handleSubInstruction(var1, var2, var3);
            } else if (opcode == "LSS") {
                std::istringstream paramStream(params);
                std::string var1, var2;
                std::getline(paramStream, var1, ',');
                std::getline(paramStream, var2, ',');

                if (var1.empty() || var2.empty()) {
                    std::cerr << "LSS instruction has incomplete parameters." << std::endl;
                    return;
                }
                currentBranchState = currentBranchState && handleLssInstruction(var1, var2);
            } else if (opcode == "GTR") {
                std::istringstream paramStream(params);
                std::string var1, var2;
                std::getline(paramStream, var1, ',');
                std::getline(paramStream, var2, ',');

                if (var1.empty() || var2.empty()) {
                    std::cerr << "GTR instruction has incomplete parameters." << std::endl;
                    return;
                }
                currentBranchState = currentBranchState && handleGtrInstruction(var1, var2);
            } else if (opcode == "BST") {
                branchStack.push(branchResult);
                branchResult = false;
                currentBranchState = true;
                std::cout << "Branch Start, pushing current branch result to stack." << std::endl;
            } else if (opcode == "NXB") {
                branchResult = branchResult || currentBranchState;
                currentBranchState = true;
                std::cout << "Next Branch, updating branch result to " << boolToString(branchResult) << std::endl;
            } else if (opcode == "BND") {
                branchResult = branchResult || currentBranchState;
                currentBranchState = branchStack.top();
                branchStack.pop();
                currentBranchState = currentBranchState && branchResult;
                std::cout << "Branch End, updating branch result to " << boolToString(branchResult) << std::endl;
            } else {
                std::cerr << "Unknown instruction: " << instruction << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << opcode << " instruction error: " << e.what() << std::endl;
        }
        ++i;
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

bool LadderLogicParser::handleLssInstruction(const std::string& var1, const std::string& var2) {
    if (variableMap.find(var1) != variableMap.end() && variableMap.find(var2) != variableMap.end()) {
        bool result;
        if (std::holds_alternative<int>(variableMap[var1]) && std::holds_alternative<int>(variableMap[var2])) {
            int val1 = std::get<int>(variableMap[var1]);
            int val2 = std::get<int>(variableMap[var2]);
            result = val1 < val2;
            std::cout << "LSS(" << var1 << "," << var2 << ") = LSS(" << val1 << "," << val2 << ") = " << boolToString(result) << std::endl;
        } else if (std::holds_alternative<double>(variableMap[var1]) && std::holds_alternative<double>(variableMap[var2])) {
            double val1 = std::get<double>(variableMap[var1]);
            double val2 = std::get<double>(variableMap[var2]);
            result = val1 < val2;
            std::cout << "LSS(" << var1 << "," << var2 << ") = LSS(" << val1 << "," << val2 << ") = " << boolToString(result) << std::endl;
        } else {
            std::cerr << "LSS instruction type mismatch: " << var1 << ", " << var2 << std::endl;
            return false;
        }
        return result;
    } else {
        std::cerr << "LSS instruction variables not found: " << var1 << ", " << var2 << std::endl;
        return false;
    }
}

bool LadderLogicParser::handleGtrInstruction(const std::string& var1, const std::string& var2) {
    if (variableMap.find(var1) != variableMap.end() && variableMap.find(var2) != variableMap.end()) {
        bool result;
        if (std::holds_alternative<int>(variableMap[var1]) && std::holds_alternative<int>(variableMap[var2])) {
            int val1 = std::get<int>(variableMap[var1]);
            int val2 = std::get<int>(variableMap[var2]);
            result = val1 > val2;
            std::cout << "GTR(" << var1 << "," << var2 << ") = GTR(" << val1 << "," << val2 << ") = " << boolToString(result) << std::endl;
        } else if (std::holds_alternative<double>(variableMap[var1]) && std::holds_alternative<double>(variableMap[var2])) {
            double val1 = std::get<double>(variableMap[var1]);
            double val2 = std::get<double>(variableMap[var2]);
            result = val1 > val2;
            std::cout << "GTR(" << var1 << "," << var2 << ") = GTR(" << val1 << "," << val2 << ") = " << boolToString(result) << std::endl;
        } else {
            std::cerr << "GTR instruction type mismatch: " << var1 << ", " << var2 << std::endl;
            return false;
        }
        return result;
    } else {
        std::cerr << "GTR instruction variables not found: " << var1 << ", " << var2 << std::endl;
        return false;
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
