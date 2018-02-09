#pragma once

#include "Includes.h"

class CommandBase
{
public:
    CommandBase() 
    {
        
    }
    
    virtual bool Execute() = 0;
};