#pragma once

#include "CommandInvoker.h"
#include "Includes.h"

class CommandInvoker
{
public:
    void SetCommand(Base Command)
    {
        this->Command = Command;
    }
    
    bool RunCommand(std::vector<char *> &ArgumentList)
    {
        return Command->Execute(args);
    }
    
private:
    Base Command;
};