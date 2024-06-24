#ifndef LADDER_LOGIC_INTERPRETER_H
#define LADDER_LOGIC_INTERPRETER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

class LadderLogicInterpreter {
public:
    LadderLogicInterpreter();
    void parseLadderLogic(const std::string &ladderLogic);
    void executeLadderLogic();
    void printRelayStatus() const;

    void setRelayState(const std::string &name, bool state);

    enum DataType { INT, BOOL, REAL };

    void assignDataType(const std::string &name, DataType type, const std::string &value = "0");
    void unassignDataType(const std::string &name);

    void loadVariablesFromFile(const std::string &filename);
    void saveVariablesToFile(const std::string &filename) const;

private:
    struct Relay {
        std::string name;
        bool state = false;
    };

    struct Instruction {
        std::string type;  // XIC, XIO, OTE, OTL, OTU, ADD, etc.
        std::vector<std::string> operands;
    };

    std::map<std::string, Relay> relays;
    std::vector<Instruction> instructions;

    std::map<std::string, int> intVariables;
    std::map<std::string, float> realVariables;
    std::map<std::string, bool> boolVariables;

    bool getRelayState(const std::string &name) const;
    void saveVariable(std::ofstream &out, const std::string &name, DataType type) const;
    void executeInstruction(const Instruction &instruction);

    template <typename T>
    T getVariableValue(const std::string &name) const;

    template <typename T>
    void setVariableValue(const std::string &name, T value);

    DataType getVariableType(const std::string &name) const;
    void ensureVariableExists(const std::string &name, DataType type);
};

#endif // LADDER_LOGIC_INTERPRETER_H
