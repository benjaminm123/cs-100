#pragma once

#include "CommandInvoker.h"
#include "Includes.h"

class CommandInvoker
{
public:
    void SetCommand(CommandBase *Command)
    {
        this->Command = Command;
    }
    
    bool RunCommand()
    {
        return Command->Execute();
    }
    
private:
    CommandBase *Command;
};