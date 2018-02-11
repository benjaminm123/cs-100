#pragma once

#include "CommandBase.h"
#include "CommandReceiver.h"

#include <memory>
#include <string>
#include <iostream>

class Exit : public CommandBase
{
public:
    Exit(std::shared_ptr<CommandReceiver> Receiver) : Receiver(Receiver)
    {
    
    }
    
    bool Execute()
    {
        Receiver->Terminate();
        
        return true;
    }
    
    std::string GetCommand(int i) const
    {
        return NULL;
    }

private:
    std::shared_ptr<CommandReceiver> Receiver;
};