#pragma once

#include "CommandInvoker.h"
#include "Includes.h"

class CommandInvoker
{
public:
    void SetCommand(std::shared_ptr<CommandBase> Command)
    {
        this->Command = Command;
    }
    
    bool RunCommand()
    {
        return Command->Execute();
    }
    
private:
    std::shared_ptr<CommandBase> Command;
};