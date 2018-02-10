#pragma once

#include "ConcreteConnector.h"
#include "ConcreteSingleCommand.h"

class Parser
{
public:
    Parser(std::string &Input);
    void ParseInput(std::vector<std::string> &InputVec, std::queue<std::string> &ConnectorList);
    void ParseCommands(std::queue<std::string> &ConnectorList);
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
};