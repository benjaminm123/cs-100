#pragma once

#include "ConcreteConnector.h"
#include "ConcreteSingleCommand.h"

class Parser
{
public:
    Parser(std::string &Input);
    bool ParseInput(std::vector<std::string> &InputVec, std::queue<std::string> &ConnectorList);
    void ParseConnectors(std::queue<std::string> &ConnectorList);
    void SpaceInput();
    void SplitInput();
    void ParseQuotations();
    void BalanceQuotations();
    void BalanceOperators();
    bool CheckSyntax();
    
private:
    std::string Input;
    std::vector<std::string> InputList;
    std::vector<unsigned int> SavePosition;
    std::vector<std::pair<std::string, unsigned int>> SaveIndex;
    std::vector<std::string> OriginalInputList;
    std::vector<std::pair<unsigned int, unsigned int>> SaveQuotation;
    std::string SaveInput;
};