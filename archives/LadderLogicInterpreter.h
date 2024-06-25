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
    void printVariables() const;
    void saveDataTypesToFile(const std::string &filename) const;
    void loadDataTypesFromFile(const std::string &filename);



    void setRelayState(const std::string &name, bool state);

    enum DataType { INT, DINT, REAL, BOOL };

    void assignDataType(const std::string &name, DataType type, const std::string &value);
    void unassignDataType(const std::string &name);

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
    std::map<std::string, int> dintVariables;
    std::map<std::string, float> realVariables;
    std::map<std::string, bool> boolVariables;

    bool getRelayState(const std::string &name) const;
    void saveVariable(std::ofstream &out, const std::string &name, DataType type) const;
    void executeInstruction(const Instruction &instruction);

    template <typename T>
    T getVariableValue(const std::string &name) const;

    template <typename T>
    void setVariableValue(const std::string &name, T value);
};

#endif // LADDER_LOGIC_INTERPRETER_H
