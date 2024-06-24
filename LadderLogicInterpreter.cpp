#include "LadderLogicInterpreter.h"

LadderLogicInterpreter::LadderLogicInterpreter() {
    // Initialize your arrays or any other setup
}

void LadderLogicInterpreter::parseLadderLogic(const std::string &ladderLogic) {
    std::istringstream stream(ladderLogic);
    std::string line;
    while (std::getline(stream, line)) {
        std::istringstream lineStream(line);
        std::string token;
        lineStream >> token;
        
        if (token == "VAR") {
            std::string name, type, value;
            lineStream >> name >> type >> value;
            assignDataType(name, type == "INT" ? INT : type == "DINT" ? DINT : type == "REAL" ? REAL : BOOL, value);
        } else {
            Instruction instruction;
            instruction.type = token;
            while (lineStream >> token) {
                instruction.operands.push_back(token);
            }
            instructions.push_back(instruction);
        }
    }
}

void LadderLogicInterpreter::executeLadderLogic() {
    for (const auto &instruction : instructions) {
        executeInstruction(instruction);
    }
}

void LadderLogicInterpreter::executeInstruction(const Instruction &instruction) {
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
        float val1 = getVariableValue<float>(instruction.operands[0]);
        float val2 = getVariableValue<float>(instruction.operands[1]);
        setVariableValue(instruction.operands[2], val1 + val2);
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
}

void LadderLogicInterpreter::assignDataType(const std::string &name, DataType type, const std::string &value) {
    switch (type) {
        case INT:
            intVariables[name] = std::stoi(value);
            break;
        case DINT:
            dintVariables[name] = std::stoi(value);
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
    dintVariables.erase(name);
    realVariables.erase(name);
    boolVariables.erase(name);
}

void LadderLogicInterpreter::saveVariable(std::ofstream &out, const std::string &name, DataType type) const {
    switch (type) {
        case INT:
            out << "VAR " << name << " INT " << intVariables.at(name) << "\n";
            break;
        case DINT:
            out << "VAR " << name << " DINT " << dintVariables.at(name) << "\n";
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
        if (intVariables.find(name) != intVariables.end()) {
            return intVariables.at(name);
        } else if (dintVariables.find(name) != dintVariables.end()) {
            return dintVariables.at(name);
        }
    } else if (std::is_same<T, float>::value) {
        return realVariables.at(name);
    } else if (std::is_same<T, bool>::value) {
        return boolVariables.at(name);
    }
    return T{};
}

template <typename T>
void LadderLogicInterpreter::setVariableValue(const std::string &name, T value) {
    if (std::is_same<T, int>::value) {
        if (intVariables.find(name) != intVariables.end()) {
            intVariables[name] = value;
        } else if (dintVariables.find(name) != dintVariables.end()) {
            dintVariables[name] = value;
        }
    } else if (std::is_same<T, float>::value) {
        realVariables[name] = value;
    } else if (std::is_same<T, bool>::value) {
        boolVariables[name] = value;
    }
}

void LadderLogicInterpreter::saveDataTypesToFile(const std::string &filename) const {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    for (const auto &var : intVariables) {
        saveVariable(out, var.first, INT);
    }
    for (const auto &var : dintVariables) {
        saveVariable(out, var.first, DINT);
    }
    for (const auto &var : realVariables) {
        saveVariable(out, var.first, REAL);
    }
    for (const auto &var : boolVariables) {
        saveVariable(out, var.first, BOOL);
    }

    out.close();
}

void LadderLogicInterpreter::loadDataTypesFromFile(const std::string &filename) {
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
        } else if (type == "DINT") {
            dintVariables[name] = std::stoi(value);
        } else if (type == "REAL") {
            realVariables[name] = std::stof(value);
        } else if (type == "BOOL") {
            boolVariables[name] = (value == "1");
        }
    }

    in.close();
}
