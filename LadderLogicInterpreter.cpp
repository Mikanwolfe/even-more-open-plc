#include "LadderLogicInterpreter.h"

LadderLogicInterpreter::LadderLogicInterpreter() {
    // Initialize your arrays or any other setup
}

void LadderLogicInterpreter::parseLadderLogic(const std::string &ladderLogic) {
    std::istringstream stream(ladderLogic);
    std::string line;
    std::cout << "Parsing ladder logic..." << std::endl;
    while (std::getline(stream, line)) {
        std::istringstream lineStream(line);
        std::string token;
        lineStream >> token;
        
        Instruction instruction;
        instruction.type = token;
        while (lineStream >> token) {
            instruction.operands.push_back(token);
        }
        instructions.push_back(instruction);
        std::cout << "Parsed instruction: " << instruction.type;
        for (const auto &operand : instruction.operands) {
            std::cout << " " << operand;
        }
        std::cout << std::endl;
    }
}

void LadderLogicInterpreter::executeLadderLogic() {
    std::cout << "Executing ladder logic..." << std::endl;
    for (const auto &instruction : instructions) {
        executeInstruction(instruction);
    }
}

void LadderLogicInterpreter::executeInstruction(const Instruction &instruction) {
    std::cout << "Executing instruction: " << instruction.type;
    for (const auto &operand : instruction.operands) {
        std::cout << " " << operand;
    }
    std::cout << std::endl;

    if (instruction.type == "BST" || instruction.type == "BND") {
        // Handle branch start and end if needed
    } else if (instruction.type == "XIC") {
        bool result = getRelayState(instruction.operands[0]);
        setRelayState(instruction.operands[1], result);
    } else if (instruction.type == "XIO") {
        bool result = !getRelayState(instruction.operands[0]);
        setRelayState(instruction.operands[1], result);
    } else if (instruction.type == "OTE") {
        setRelayState(instruction.operands[0], getRelayState(instruction.operands[1]));
    } else if (instruction.type == "ADD") {
        ensureVariableExists(instruction.operands[0], REAL);
        ensureVariableExists(instruction.operands[1], REAL);
        ensureVariableExists(instruction.operands[2], REAL);

        float val1 = getVariableValue<float>(instruction.operands[0]);
        float val2 = getVariableValue<float>(instruction.operands[1]);
        float result = val1 + val2;
        setVariableValue(instruction.operands[2], result);
        std::cout << "ADD result: " << instruction.operands[2] << " = " << result << std::endl;
    }
}

void LadderLogicInterpreter::printRelayStatus() const {
    std::cout << "State of relays:\n";
    for (const auto &relay : relays) {
        std::cout << relay.second.name << ": " << (relay.second.state ? "ON" : "OFF") << "\n";
    }
}

bool LadderLogicInterpreter::getRelayState(const std::string &name) const {
    auto it = relays.find(name);
    return it != relays.end() ? it->second.state : false;
}

void LadderLogicInterpreter::setRelayState(const std::string &name, bool state) {
    relays[name].name = name;
    relays[name].state = state;
    std::cout << "Relay " << name << " set to " << (state ? "ON" : "OFF") << std::endl;
}

void LadderLogicInterpreter::assignDataType(const std::string &name, DataType type, const std::string &value) {
    std::cout << "Assigning variable: " << name << " of type " << (type == INT ? "INT" : type == REAL ? "REAL" : "BOOL") << " with value " << value << std::endl;
    switch (type) {
        case INT:
            intVariables[name] = std::stoi(value);
            break;
        case REAL:
            realVariables[name] = std::stof(value);
            break;
        case BOOL:
            boolVariables[name] = (value == "1");
            break;
    }
}

void LadderLogicInterpreter::unassignDataType(const std::string &name) {
    intVariables.erase(name);
    realVariables.erase(name);
    boolVariables.erase(name);
}

void LadderLogicInterpreter::saveVariable(std::ofstream &out, const std::string &name, DataType type) const {
    switch (type) {
        case INT:
            out << "VAR " << name << " INT " << intVariables.at(name) << "\n";
            break;
        case REAL:
            out << "VAR " << name << " REAL " << realVariables.at(name) << "\n";
            break;
        case BOOL:
            out << "VAR " << name << " BOOL " << boolVariables.at(name) << "\n";
            break;
    }
}

template <typename T>
T LadderLogicInterpreter::getVariableValue(const std::string &name) const {
    if (std::is_same<T, int>::value) {
        return intVariables.at(name);
    } else if (std::is_same<T, float>::value) {
        return realVariables.at(name);
    } else if (std::is_same<T, bool>::value) {
        return boolVariables.at(name);
    }
    return T{};
}

template <typename T>
void LadderLogicInterpreter::setVariableValue(const std::string &name, T value) {
    std::cout << "Setting variable: " << name << " with value " << value << std::endl;
    if (std::is_same<T, int>::value) {
        intVariables[name] = value;
    } else if (std::is_same<T, float>::value) {
        realVariables[name] = value;
    } else if (std::is_same<T, bool>::value) {
        boolVariables[name] = value;
    }
}

LadderLogicInterpreter::DataType LadderLogicInterpreter::getVariableType(const std::string &name) const {
    if (intVariables.find(name) != intVariables.end()) {
        return INT;
    } else if (realVariables.find(name) != realVariables.end()) {
        return REAL;
    } else if (boolVariables.find(name) != boolVariables.end()) {
        return BOOL;
    }
    return INT; // Default type
}

void LadderLogicInterpreter::ensureVariableExists(const std::string &name, DataType type) {
    if (intVariables.find(name) == intVariables.end() &&
        realVariables.find(name) == intVariables.end() &&
        boolVariables.find(name) == boolVariables.end()) {
        assignDataType(name, type);
    }
}

void LadderLogicInterpreter::saveVariablesToFile(const std::string &filename) const {
    std::cout << "Saving variables to file: " << filename << std::endl;
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    for (const auto &var : intVariables) {
        saveVariable(out, var.first, INT);
    }
    for (const auto &var : realVariables) {
        saveVariable(out, var.first, REAL);
    }
    for (const auto &var : boolVariables) {
        saveVariable(out, var.first, BOOL);
    }

    out.close();
}

void LadderLogicInterpreter::loadVariablesFromFile(const std::string &filename) {
    std::cout << "Loading variables from file: " << filename << std::endl;
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        std::string type, name, value;
        iss >> type >> name >> type >> value;
        if (type == "INT") {
            intVariables[name] = std::stoi(value);
        } else if (type == "REAL") {
            realVariables[name] = std::stof(value);
        } else if (type == "BOOL") {
            boolVariables[name] = (value == "1");
        }
        std::cout << "Loaded variable: " << name << " of type " << type << " with value " << value << std::endl;
    }

    in.close();
}

void LadderLogicInterpreter::printVariableStatus() const {
    std::cout << "State of variables:\n";
    for (const auto &var : intVariables) {
        std::cout << var.first << " (INT): " << var.second << "\n";
    }
    for (const auto &var : realVariables) {
        std::cout << var.first << " (REAL): " << var.second << "\n";
    }
    for (const auto &var : boolVariables) {
        std::cout << var.first << " (BOOL): " << (var.second ? "1" : "0") << "\n";
    }
}
