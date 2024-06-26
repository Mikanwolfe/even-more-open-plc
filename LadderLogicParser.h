#include <map>
#include <string>
#include <variant>
#include <vector>
#include <stack>
#include <chrono>

using Variable = std::variant<int, bool, double>;

class LadderLogicParser {
public:
    LadderLogicParser(const std::vector<std::string>& logic, std::map<std::string, Variable>& variableMap);
    void parseAndExecute();
    void executeLogic(); // New method to execute logic without re-initializing

    int scanTime = 0; // in milliseconds
    bool isFirstScan; // flag for the first scan


private:
    std::vector<std::string> logic;
    std::map<std::string, Variable>& variableMap;
    bool lineState;
    bool endFound;

    std::chrono::high_resolution_clock::time_point initialTime;
    double roundToTwoDecimals(double value);

    void handleTokens(const std::vector<std::string>& tokens);
    void handleInstruction(const std::string& instruction, const std::string& params, bool& currentBranchState);
    void handleBranchStart(std::stack<bool>& branchStack, std::stack<bool>& currentBranchStateStack, bool& branchResult, bool& currentBranchState);
    void handleNextBranch(bool& branchResult, bool& currentBranchState);
    void handleBranchEnd(std::stack<bool>& branchStack, std::stack<bool>& currentBranchStateStack, bool& branchResult, bool& currentBranchState);
    bool handleTonInstruction(const std::string& params, bool currentBranchState);
    bool handleTofInstruction(const std::string& params, bool currentBranchState);
    bool getBoolValue(const std::string& varName);
    void setBoolValue(const std::string& varName, bool value);

    bool handleAddInstruction(const std::string& params, bool& currentBranchState);
    bool handleSubInstruction(const std::string& params, bool& currentBranchState);
    bool handleLssInstruction(const std::string& params, bool& currentBranchState);
    bool handleGtrInstruction(const std::string& params, bool& currentBranchState);
    bool handleAfiInstruction(const std::string& params, bool& currentBranchState);
    bool handleEquInstruction(const std::string& params, bool& currentBranchState);
    bool handleNeqInstruction(const std::string& params, bool& currentBranchState);
    bool handleOnrInstruction(const std::string& params, bool& currentBranchState);
    bool handleOnfInstruction(const std::string& params, bool& currentBranchState);
    bool handleCtuInstruction(const std::string& params, bool& currentBranchState);
    bool handleCtdInstruction(const std::string& params, bool& currentBranchState);
    bool handleXicInstruction(const std::string& params, bool& currentBranchState);
    bool handleXioInstruction(const std::string& params, bool& currentBranchState);
    bool handleOteInstruction(const std::string& params, bool& currentBranchState);
    bool handleOtlInstruction(const std::string& params, bool& currentBranchState);

};
