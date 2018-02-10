#pragma once

#include "ConcreteSingleCommand.h"
#include "ConcreteConnector.h"
#include "CommandInvoker.h"
#include "Parser.h"

class Shell
{
public:
    Shell();
    void Run();
    void PrintPrompt(bool &InfoAvailable);
    bool ExecuteCommands();
    void BuildTree();
    void CreateCommands();
    
private:
    std::shared_ptr<CommandReceiver> Receiver;
    std::queue<std::shared_ptr<CommandBase>> SingleCommandList;
    std::deque<std::shared_ptr<CommandBase>> CommandTree;
    std::queue<std::string> ConnectorList;
    std::shared_ptr<CommandInvoker> Invoker;
    std::shared_ptr<ConcreteSingleCommand> Command;
    std::shared_ptr<ConcreteConnector> Connector;
    std::vector<std::string> InputVec;
    int NumCommands;
};