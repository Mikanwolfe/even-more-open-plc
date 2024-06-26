#include "LadderLogicParser.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stack>
#include <thread>
#include <chrono>
#include <cmath>

LadderLogicParser::LadderLogicParser(const std::vector<std::string>& logic, std::map<std::string, Variable>& variableMap)
    : logic(logic), variableMap(variableMap), lineState(true), endFound(false) {}

std::string boolToString(bool value) {
    return value ? "true" : "false";
}

double LadderLogicParser::roundToTwoDecimals(double value) {
    return std::round(value * 100.0) / 100.0;
}


void LadderLogicParser::parseAndExecute() {
    initialTime = std::chrono::high_resolution_clock::now();
    executeLogic();
}

void LadderLogicParser::executeLogic() {
    using namespace std::chrono;

    auto start = high_resolution_clock::now();
    
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
        std::cout << "| ===  ";
        handleTokens(tokens);
        std::cout << "|" << std::endl;
    }

    
    // Simulate a delay between scans
    // std::this_thread::sleep_for(milliseconds(1));
    auto end = high_resolution_clock::now();
    scanTime = duration_cast<microseconds>(end - start).count();
}

void LadderLogicParser::handleTokens(const std::vector<std::string>& tokens) {
    std::stack<bool> branchStack;
    std::stack<bool> currentBranchStateStack;
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
            } else if (opcode == "BST") {
                handleBranchStart(branchStack, currentBranchStateStack, branchResult, currentBranchState);
            } else if (opcode == "NXB") {
                handleNextBranch(branchResult, currentBranchState);
            } else if (opcode == "BND") {
                handleBranchEnd(branchStack, currentBranchStateStack, branchResult, currentBranchState);
            } else {
                handleInstruction(opcode, params, currentBranchState);
            }
        } catch (const std::exception& e) {
            std::cerr << opcode << " instruction error: " << e.what() << std::endl;
        }
        ++i;
    }
}
void LadderLogicParser::handleInstruction(const std::string& opcode, const std::string& params, bool& currentBranchState) {
    if (opcode == "XIC") {
        currentBranchState = currentBranchState && handleXicInstruction(params, currentBranchState);
    } else if (opcode == "XIO") {
        currentBranchState = currentBranchState && handleXioInstruction(params, currentBranchState);
    } else if (opcode == "OTE") {
        currentBranchState = currentBranchState && handleOteInstruction(params, currentBranchState);
    } else if (opcode == "OTL") {
        currentBranchState = currentBranchState && handleOtlInstruction(params, currentBranchState);
    } else if (opcode == "AFI") {
        currentBranchState = currentBranchState && handleAfiInstruction(params, currentBranchState);
    } else if (opcode == "ADD") {
        currentBranchState = currentBranchState && handleAddInstruction(params, currentBranchState);
    } else if (opcode == "SUB") {
        currentBranchState = currentBranchState && handleSubInstruction(params, currentBranchState);
    } else if (opcode == "LSS") {
        currentBranchState = currentBranchState && handleLssInstruction(params, currentBranchState);
    } else if (opcode == "GTR") {
        currentBranchState = currentBranchState && handleGtrInstruction(params, currentBranchState);
    } else if (opcode == "EQU") {
        currentBranchState = currentBranchState && handleEquInstruction(params, currentBranchState);
    } else if (opcode == "NEQ") {
        currentBranchState = currentBranchState && handleNeqInstruction(params, currentBranchState);
    } else if (opcode == "CTU") {
        currentBranchState = currentBranchState && handleCtuInstruction(params, currentBranchState);
    } else if (opcode == "CTD") {
        currentBranchState = currentBranchState && handleCtdInstruction(params, currentBranchState);
    } else if (opcode == "TON") {
        currentBranchState = currentBranchState && handleTonInstruction(params, currentBranchState);
    } else if (opcode == "TOF") {
        currentBranchState = currentBranchState && handleTofInstruction(params, currentBranchState);
    } else if (opcode == "ONR") {
        currentBranchState = currentBranchState && handleOnrInstruction(params, currentBranchState);
    } else if (opcode == "ONF") {
        currentBranchState = currentBranchState && handleOnfInstruction(params, currentBranchState);
    } else {
        std::cerr << "Unknown instruction: " << opcode << std::endl;
    }
}


bool LadderLogicParser::handleXicInstruction(const std::string& params, bool& currentBranchState) {
    if (params.empty()) {
        std::cerr << "XIC instruction missing parameters." << std::endl;
        return currentBranchState;
    }
    bool value = getBoolValue(params);
    currentBranchState = currentBranchState && value;
    std::cout << "XIC[" << params << "]" << (currentBranchState ? " === " : " --- ");
    return currentBranchState;
}

bool LadderLogicParser::handleXioInstruction(const std::string& params, bool& currentBranchState) {
    if (params.empty()) {
        std::cerr << "XIO instruction missing parameters." << std::endl;
        return currentBranchState;
    }
    bool value = !getBoolValue(params);
    currentBranchState = currentBranchState && value;
    std::cout << "XIO[" << params << "]" << (currentBranchState ? " === " : " --- ");
    return currentBranchState;
}

bool LadderLogicParser::handleOteInstruction(const std::string& params, bool& currentBranchState) {
    if (params.empty()) {
        std::cerr << "OTE instruction missing parameters." << std::endl;
        return currentBranchState;
    }
    setBoolValue(params, currentBranchState);
    std::cout << "OTE[" << params << "]" << (currentBranchState ? " === " : " --- ");
    return currentBranchState;
}

bool LadderLogicParser::handleOtlInstruction(const std::string& params, bool& currentBranchState) {
    if (params.empty()) {
        std::cerr << "OTL instruction missing parameters." << std::endl;
        return currentBranchState;
    }
    if (currentBranchState) {
        setBoolValue(params, true);
    }
    std::cout << "OTL[" << params << "]" << (getBoolValue(params) ? " === " : " --- ");
    return currentBranchState;
}

bool LadderLogicParser::handleEquInstruction(const std::string& params, bool& currentBranchState) {
    std::istringstream paramStream(params);
    std::string var1, var2;
    std::getline(paramStream, var1, ',');
    std::getline(paramStream, var2, ',');

    if (var1.empty() || var2.empty()) {
        std::cerr << "EQU instruction has incomplete parameters." << std::endl;
        return false;
    }

    if (variableMap.find(var1) != variableMap.end() && variableMap.find(var2) != variableMap.end()) {
        bool result;
        if (std::holds_alternative<int>(variableMap[var1]) && std::holds_alternative<int>(variableMap[var2])) {
            int val1 = std::get<int>(variableMap[var1]);
            int val2 = std::get<int>(variableMap[var2]);
            result = val1 == val2;
            std::cout << "EQU(" << val1 << " == " << val2 << ")" << (result ? " === " : " --- ");
        } else if (std::holds_alternative<double>(variableMap[var1]) && std::holds_alternative<double>(variableMap[var2])) {
            double val1 = roundToTwoDecimals(std::get<double>(variableMap[var1]));
            double val2 = roundToTwoDecimals(std::get<double>(variableMap[var2]));
            result = val1 == val2;
            std::cout << "EQU(" << val1 << " == " << val2 << ")" << (result ? " === " : " --- ");
        } else {
            std::cerr << "EQU instruction type mismatch: " << var1 << ", " << var2 << std::endl;
            return false;
        }
        return result;
    } else {
        std::cerr << "EQU instruction variables not found: " << var1 << ", " << var2 << std::endl;
        return false;
    }
}

bool LadderLogicParser::handleAfiInstruction(const std::string& params, bool& currentBranchState) {
    currentBranchState = false;
    std::cout << "AFI" << (currentBranchState ? " === " : " --- ");
    return false;
}


bool LadderLogicParser::handleNeqInstruction(const std::string& params, bool& currentBranchState) {
    std::istringstream paramStream(params);
    std::string var1, var2;
    std::getline(paramStream, var1, ',');
    std::getline(paramStream, var2, ',');

    if (var1.empty() || var2.empty()) {
        std::cerr << "NEQ instruction has incomplete parameters." << std::endl;
        return false;
    }

    if (variableMap.find(var1) != variableMap.end() && variableMap.find(var2) != variableMap.end()) {
        bool result;
        if (std::holds_alternative<int>(variableMap[var1]) && std::holds_alternative<int>(variableMap[var2])) {
            int val1 = std::get<int>(variableMap[var1]);
            int val2 = std::get<int>(variableMap[var2]);
            result = val1 != val2;
            std::cout << "NEQ(" << val1 << " != " << val2 << ")" << (result ? " === " : " --- ");
        } else if (std::holds_alternative<double>(variableMap[var1]) && std::holds_alternative<double>(variableMap[var2])) {
            double val1 = roundToTwoDecimals(std::get<double>(variableMap[var1]));
            double val2 = roundToTwoDecimals(std::get<double>(variableMap[var2]));
            result = val1 != val2;
            std::cout << "NEQ(" << val1 << " != " << val2 << ")" << (result ? " === " : " --- ");
        } else {
            std::cerr << "NEQ instruction type mismatch: " << var1 << ", " << var2 << std::endl;
            return false;
        }
        return result;
    } else {
        std::cerr << "NEQ instruction variables not found: " << var1 << ", " << var2 << std::endl;
        return false;
    }
}

bool LadderLogicParser::handleCtuInstruction(const std::string& params, bool& currentBranchState) {
    std::istringstream paramStream(params);
    std::string pre, acc, ct, dn;
    std::getline(paramStream, pre, ',');
    std::getline(paramStream, acc, ',');
    std::getline(paramStream, ct, ',');
    std::getline(paramStream, dn, ',');

    if (pre.empty() || acc.empty() || ct.empty() || dn.empty()) {
        std::cerr << "CTU instruction has incomplete parameters." << std::endl;
        return currentBranchState;
    }

    int preValue = std::get<int>(variableMap[pre]);
    int accValue = std::get<int>(variableMap[acc]);
    bool ctValue = getBoolValue(ct);

    if (currentBranchState && !ctValue) {
        accValue++;
        setBoolValue(ct, true);
        std::cout << "CTU[" << params << "] === ";
    } else if (!currentBranchState) {
        setBoolValue(ct, false);
        std::cout << "CTU[" << params << "] --- ";
    }

    if (accValue >= preValue) {
        setBoolValue(dn, true);
    } else {
        setBoolValue(dn, false);
    }

    variableMap[acc] = accValue;
    std::cout << "ACC: " << accValue << ", DN: " << boolToString(getBoolValue(dn)) << std::endl;
    return currentBranchState;
}

bool LadderLogicParser::handleCtdInstruction(const std::string& params, bool& currentBranchState) {
    std::istringstream paramStream(params);
    std::string pre, acc, ct, dn;
    std::getline(paramStream, pre, ',');
    std::getline(paramStream, acc, ',');
    std::getline(paramStream, ct, ',');
    std::getline(paramStream, dn, ',');

    if (pre.empty() || acc.empty() || ct.empty() || dn.empty()) {
        std::cerr << "CTD instruction has incomplete parameters." << std::endl;
        return currentBranchState;
    }

    int accValue = std::get<int>(variableMap[acc]);
    bool ctValue = getBoolValue(ct);

    if (!currentBranchState && ctValue) {
        accValue--;
        setBoolValue(ct, false);
        std::cout << "CTD[" << params << "] === ";
    } else if (currentBranchState) {
        setBoolValue(ct, true);
        std::cout << "CTD[" << params << "] --- ";
    }

    if (accValue <= 0) {
        setBoolValue(dn, true);
    } else {
        setBoolValue(dn, false);
    }

    variableMap[acc] = accValue;
    std::cout << "ACC: " << accValue << ", DN: " << boolToString(getBoolValue(dn)) << std::endl;
    return currentBranchState;
}

bool LadderLogicParser::handleOnrInstruction(const std::string& var1, bool& currentBranchState) {
    bool previousState = getBoolValue(var1);
    if (currentBranchState && !previousState) {
        setBoolValue(var1, true);
        currentBranchState = true;
        std::cout << "ONR[" << var1 << "]" << (currentBranchState ? " === " : " --- ");
        return true;
    }
    setBoolValue(var1, currentBranchState);
    currentBranchState = false;
    std::cout << "ONR[" << var1 << "]" << (currentBranchState ? " === " : " --- ");
    return false;
}

bool LadderLogicParser::handleOnfInstruction(const std::string& var1, bool& currentBranchState) {
    bool previousState = getBoolValue(var1);
    if (!currentBranchState && previousState) {
        setBoolValue(var1, false);
        currentBranchState = true;
        std::cout << "ONF[" << var1 << "]" << (currentBranchState ? " === " : " --- ");
        return true;
    }
    setBoolValue(var1, currentBranchState);
    currentBranchState = false;
    std::cout << "ONF[" << var1 << "]" << (currentBranchState ? " === " : " --- ");
    return false;
}

void LadderLogicParser::handleBranchStart(std::stack<bool>& branchStack, std::stack<bool>& currentBranchStateStack, bool& branchResult, bool& currentBranchState) {
    branchStack.push(branchResult);
    currentBranchStateStack.push(currentBranchState);
    branchResult = false;
    currentBranchState = true;
    std::cout << "<<" << std::endl;
}

void LadderLogicParser::handleNextBranch(bool& branchResult, bool& currentBranchState) {
    branchResult = branchResult || currentBranchState;
    currentBranchState = true;
    std::cout << "^^" << std::endl;
}

void LadderLogicParser::handleBranchEnd(std::stack<bool>& branchStack, std::stack<bool>& currentBranchStateStack, bool& branchResult, bool& currentBranchState) {
    branchResult = branchResult || currentBranchState;
    currentBranchState = currentBranchStateStack.top();
    currentBranchStateStack.pop();
    branchResult = branchResult && currentBranchState;
    currentBranchState = branchStack.top();
    branchStack.pop();
    currentBranchState = currentBranchState && branchResult;
    std::cout << ">>" << std::endl;
}

bool LadderLogicParser::handleTonInstruction(const std::string& params, bool& currentBranchState) {
    std::istringstream paramStream(params);
    std::string dn, tt, pre, acc;
    std::getline(paramStream, dn, ',');
    std::getline(paramStream, tt, ',');
    std::getline(paramStream, pre, ',');
    std::getline(paramStream, acc, ',');

    if (dn.empty() || tt.empty() || pre.empty() || acc.empty()) {
        std::cerr << "TON instruction has incomplete parameters." << std::endl;
        return currentBranchState;
    }

    int preValue = std::get<int>(variableMap[pre]);
    int accValue = std::get<int>(variableMap[acc]);

    if (currentBranchState) {
        variableMap[tt] = true;
        accValue += scanTime;
        if (accValue >= preValue) {
            accValue = preValue;
            variableMap[dn] = true;
            variableMap[tt] = false;
        } else {
            variableMap[dn] = false;
        }
    } else {
        accValue = 0;
        variableMap[tt] = false;
        variableMap[dn] = false;
    }

    variableMap[acc] = accValue;
    std::cout << "TON(" << accValue << "/" << preValue << ")" << (currentBranchState ? " === " : " --- ");
    return currentBranchState;
}

bool LadderLogicParser::handleTofInstruction(const std::string& params, bool& currentBranchState) {
    std::istringstream paramStream(params);
    std::string dn, tt, pre, acc;
    std::getline(paramStream, dn, ',');
    std::getline(paramStream, tt, ',');
    std::getline(paramStream, pre, ',');
    std::getline(paramStream, acc, ',');

    if (dn.empty() || tt.empty() || pre.empty() || acc.empty()) {
        std::cerr << "TOF instruction has incomplete parameters." << std::endl;
        return currentBranchState;
    }

    int preValue = std::get<int>(variableMap[pre]);
    int accValue = std::get<int>(variableMap[acc]);

    if (!currentBranchState) {
        variableMap[tt] = true;
        accValue += scanTime;
        if (accValue >= preValue) {
            accValue = preValue;
            variableMap[dn] = false;
            variableMap[tt] = false;
        } else {
            variableMap[dn] = true;
        }
    } else {
        accValue = 0;
        variableMap[tt] = false;
        variableMap[dn] = true;
    }

    variableMap[acc] = accValue;
    std::cout << "TOF(" << accValue << "/" << preValue << ")" << (currentBranchState ? " === " : " --- ");
    return currentBranchState;
}

bool LadderLogicParser::handleAddInstruction(const std::string& params, bool& currentBranchState) {
    std::istringstream paramStream(params);
    std::string var1, var2, var3;
    std::getline(paramStream, var1, ',');
    std::getline(paramStream, var2, ',');
    std::getline(paramStream, var3, ',');

    if (var1.empty() || var2.empty() || var3.empty()) {
        std::cerr << "ADD instruction has incomplete parameters." << std::endl;
        return currentBranchState;
    }

    if (!currentBranchState) {
        std::cout << "ADD(" << var1 << " + " << var2 << ")" << " --- ";
        return currentBranchState;
    }


    if (variableMap.find(var1) != variableMap.end() && variableMap.find(var2) != variableMap.end()) {
        if (std::holds_alternative<int>(variableMap[var1]) && std::holds_alternative<int>(variableMap[var2])) {
            int result = std::get<int>(variableMap[var1]) + std::get<int>(variableMap[var2]);
            variableMap[var3] = result;
            std::cout << "ADD(" << var1 << " + " << var2 << " = " << result << ")" << " === ";
        } else if (std::holds_alternative<double>(variableMap[var1]) && std::holds_alternative<double>(variableMap[var2])) {
            double result = std::get<double>(variableMap[var1]) + std::get<double>(variableMap[var2]);
            variableMap[var3] = result;
            std::cout << "ADD(" << var1 << " + " << var2 << " = " << result << ")" << " === ";
        } else {
            std::cerr << "ADD instruction type mismatch: " << var1 << ", " << var2 << std::endl;
        }
    } else {
        std::cerr << "ADD instruction variables not found: " << var1 << ", " << var2 << std::endl;
    }
    return currentBranchState;
}

bool LadderLogicParser::handleSubInstruction(const std::string& params, bool& currentBranchState) {

        std::istringstream paramStream(params);
        std::string var1, var2, var3;
        std::getline(paramStream, var1, ',');
        std::getline(paramStream, var2, ',');
        std::getline(paramStream, var3, ',');

        if (var1.empty() || var2.empty() || var3.empty()) {
            std::cerr << "SUB instruction has incomplete parameters." << std::endl;
            return currentBranchState;
        }

        if (!currentBranchState) {
            std::cout << "SUB(" << var1 << " - " << var2 << ")" << " --- ";
            return currentBranchState;
        }

    if (variableMap.find(var1) != variableMap.end() && variableMap.find(var2) != variableMap.end()) {
        if (std::holds_alternative<int>(variableMap[var1]) && std::holds_alternative<int>(variableMap[var2])) {
            int result = std::get<int>(variableMap[var1]) - std::get<int>(variableMap[var2]);
            variableMap[var3] = result;
            std::cout << "SUB(" << var1 << " - " << var2 << " = " << result << ")" << " === ";
        } else if (std::holds_alternative<double>(variableMap[var1]) && std::holds_alternative<double>(variableMap[var2])) {
            double result = std::get<double>(variableMap[var1]) - std::get<double>(variableMap[var2]);
            variableMap[var3] = result;
            std::cout << "SUB(" << var1 << " - " << var2 << " = " << result << ")" << " === ";
        } else {
            std::cerr << "SUB instruction type mismatch: " << var1 << ", " << var2 << std::endl;
        }
    } else {
        std::cerr << "SUB instruction variables not found: " << var1 << ", " << var2 << std::endl;
    }
    return currentBranchState;
}

bool LadderLogicParser::handleLssInstruction(const std::string& params, bool& currentBranchState) {

    std::istringstream paramStream(params);
    std::string var1, var2;
    std::getline(paramStream, var1, ',');
    std::getline(paramStream, var2, ',');

    if (var1.empty() || var2.empty()) {
        std::cerr << "LSS instruction has incomplete parameters." << std::endl;
        return currentBranchState;
    }

    if (variableMap.find(var1) != variableMap.end() && variableMap.find(var2) != variableMap.end()) {
        bool result;
        if (std::holds_alternative<int>(variableMap[var1]) && std::holds_alternative<int>(variableMap[var2])) {
            int val1 = std::get<int>(variableMap[var1]);
            int val2 = std::get<int>(variableMap[var2]);
            result = val1 < val2;
            std::cout << "LSS[" << params << "]" << (currentBranchState ? " === " : " --- ");
        } else if (std::holds_alternative<double>(variableMap[var1]) && std::holds_alternative<double>(variableMap[var2])) {
            double val1 = std::get<double>(variableMap[var1]);
            double val2 = std::get<double>(variableMap[var2]);
            result = val1 < val2;
            std::cout << "LSS[" << params << "]" << (currentBranchState ? " === " : " --- ");
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

bool LadderLogicParser::handleGtrInstruction(const std::string& params, bool& currentBranchState) {

    std::istringstream paramStream(params);
    std::string var1, var2;
    std::getline(paramStream, var1, ',');
    std::getline(paramStream, var2, ',');

    if (var1.empty() || var2.empty()) {
        std::cerr << "GTR instruction has incomplete parameters." << std::endl;
        return currentBranchState;
    }

    if (variableMap.find(var1) != variableMap.end() && variableMap.find(var2) != variableMap.end()) {
        bool result;
        if (std::holds_alternative<int>(variableMap[var1]) && std::holds_alternative<int>(variableMap[var2])) {
            int val1 = std::get<int>(variableMap[var1]);
            int val2 = std::get<int>(variableMap[var2]);
            result = val1 > val2;
            std::cout << "GTR[" << params << "]" << (currentBranchState ? " === " : " --- ");
        } else if (std::holds_alternative<double>(variableMap[var1]) && std::holds_alternative<double>(variableMap[var2])) {
            double val1 = std::get<double>(variableMap[var1]);
            double val2 = std::get<double>(variableMap[var2]);
            result = val1 > val2;
            std::cout << "GTR[" << params << "]" << (currentBranchState ? " === " : " --- ");
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
