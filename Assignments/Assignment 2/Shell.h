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
    CommandReceiver* Receiver;
    std::queue<CommandBase *> SingleCommandList;
    std::deque<CommandBase *> CommandTree;
    std::queue<std::string> ConnectorList;
    CommandInvoker *Invoker;
    ConcreteSingleCommand *Command;
    ConcreteConnector *Connector;
    std::vector<std::string> InputVec;
};