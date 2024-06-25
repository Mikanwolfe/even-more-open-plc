#include <map>
#include <string>
#include <variant>
#include <vector>
#include <chrono>

using Variable = std::variant<int, bool, double>;

class LadderLogicParser {
public:
    LadderLogicParser(const std::vector<std::string>& logic, std::map<std::string, Variable>& variableMap);
    void parseAndExecute();
    void executeLogic(); // New method to execute logic without re-initializing

    void handleTokens(const std::vector<std::string>& tokens);
    int scanTime = 0; // in milliseconds
    bool isFirstScan; // flag for the first scan


private:
    std::vector<std::string> logic;
    std::map<std::string, Variable>& variableMap;
    bool lineState;
    bool endFound;

    std::chrono::high_resolution_clock::time_point initialTime;

    void handleInstruction(const std::string& instruction);
    bool getBoolValue(const std::string& varName);
    void setBoolValue(const std::string& varName, bool value);
    void handleAddInstruction(const std::string& var1, const std::string& var2, const std::string& var3);
    void handleSubInstruction(const std::string& var1, const std::string& var2, const std::string& var3);
    bool handleLssInstruction(const std::string& var1, const std::string& var2);
    bool handleGtrInstruction(const std::string& var1, const std::string& var2);
    void handleTonInstruction(const std::string& params, bool currentBranchState);
    void handleTofInstruction(const std::string& params, bool currentBranchState);
};
