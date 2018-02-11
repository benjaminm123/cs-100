#pragma once

#include "CommandBase.h"

#include <utility>
#include <vector>
#include <queue>
#include <sstream>
#include <iostream>

class Parser
{
public:
    Parser();
    Parser(std::string &Input);
    bool ParseInput(std::vector<std::string> &InputVec, std::queue<std::string> &ConnectorList);
    void ParseConnectors(std::queue<std::string> &ConnectorList);
    void SpaceInput();
    void SplitInput();
    bool ParseQuotations(std::string &TempInput, std::string &Input);
    void BalanceQuotations();
    void BalanceOperators(std::queue<std::string> &ConnectorList, bool &Ret);
    bool CheckSyntax();
    void Clear();
    void ParseComments();
    
private:
    std::string Input;
    std::string SaveInput;

    std::vector<std::string> InputList;
    std::vector<unsigned int> SavePosition;
    std::vector<std::pair<unsigned int, unsigned int>> SaveQuotation;
};