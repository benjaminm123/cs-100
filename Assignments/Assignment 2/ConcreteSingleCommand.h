#pragma once

#include "CommandBase.h"
#include "CommandReceiver.h"
#include "Includes.h"

class ConcreteSingleCommand : public CommandBase
{
public:
    ConcreteSingleCommand(Receiver Executor) : Executor(Executor)
    {
        
    }
    
    bool Execute() const
    {
        return Executor->RunProgram(ArgumentList);
    }
    
    void Push(const std::string &Argument)
    {
        ArgumentList.push_back(const_cast<char *>(Argument.c_str()));
    }
    
private:
    Receiver Executor;
    std::vector<char *> ArgumentList;
};