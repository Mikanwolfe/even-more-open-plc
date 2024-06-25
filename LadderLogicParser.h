#ifndef LADDERLOGICPARSER_H
#define LADDERLOGICPARSER_H

#include <string>
#include <vector>
#include <map>
#include <variant>

using Variable = std::variant<int, bool, double>;

class LadderLogicParser {
public:
    LadderLogicParser(const std::vector<std::string>& logic, std::map<std::string, Variable>& variableMap);
    void parseAndExecute();

private:
    std::vector<std::string> logic;
    std::map<std::string, Variable>& variableMap;
    
    void handleInstruction(const std::string& instruction);
    bool getBoolValue(const std::string& varName);
    void setBoolValue(const std::string& varName, bool value);
};

#endif // LADDERLOGICPARSER_H
