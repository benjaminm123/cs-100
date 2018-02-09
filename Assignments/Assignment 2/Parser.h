#include "Includes.h"
#include "SingleCommand.h"
#include "Connector.h"
#include "Receiver.h";

class Parser
{
public:
    Parser(std::string &Input, Receiver Executor);
    bool ParseInput(Base Command, Receiver Executor);
    void BuildTree();
    void ParseCommands(Base Command, Receiver Executor);
    void SpaceInput();
    void SplitInput();
    void ParseQuotations();
    void BalanceQuotations();
    void BalanceOperators();
    
private:
    std::string Input;
    std::vector<std::string> InputList;
    std::vector<std::pair<unsigned int, unsigned int>> SavePosition;
    std::vector<std::pair<std::string, unsigned int>> SaveIndex;
    std::queue<Base> SingleCommandList;
    std::queue<std::string> Connectors;
    std::deque<Base> CommandTree;
    Connector Connector;
    Receiver Executor;
};