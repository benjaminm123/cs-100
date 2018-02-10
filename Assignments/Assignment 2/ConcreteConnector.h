#pragma once

#include "CommandBase.h"
#include "Includes.h"

class ConcreteConnector : public CommandBase
{
public:
    ConcreteConnector()
    {
        
    }
    
    virtual bool Execute() = 0;
};

class Semicolon : public ConcreteConnector
{
public:
    Semicolon(CommandBase *lhs, CommandBase *rhs) : lhs(lhs), rhs(rhs)
    {

    }
    
    bool Execute() 
    {
        lhs->Execute();
        return rhs->Execute();
    }

private:
    CommandBase *lhs;
    CommandBase *rhs;
};

class And : public ConcreteConnector
{
public:
    And(CommandBase *lhs, CommandBase *rhs) : lhs(lhs), rhs(rhs)
    {

    }

    bool Execute() 
    {
        if (!lhs->Execute())
        {
            return false;
        }
        
        return rhs->Execute();
    }

private:
    CommandBase *lhs;
    CommandBase *rhs;
};

class Or : public ConcreteConnector
{
public:
    Or(CommandBase *lhs, CommandBase *rhs) : lhs(lhs), rhs(rhs)
    {

    }
    
    bool Execute() 
    {
        if (!lhs->Execute())
        {
            return rhs->Execute();
        }
        
        return false;
    }

private:
    CommandBase *lhs;
    CommandBase *rhs;
};