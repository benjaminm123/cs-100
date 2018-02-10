#pragma once

#include "CommandBase.h"
#include "CommandReceiver.h"

class ConcreteSingleCommand : public CommandBase
{
public:
    ConcreteSingleCommand(CommandReceiver *Receiver) : Receiver(Receiver)
    {
        
    }
    
    bool Execute()
    {
        ArgumentList.push_back(nullptr);
        return Receiver->RunProgram(ArgumentList);
    }
    
    void Push(const std::string &Argument)
    {
        ArgumentList.push_back(const_cast<char *>(Argument.c_str()));
    }
    
private:
    CommandReceiver *Receiver;
    std::vector<char *> ArgumentList;
};