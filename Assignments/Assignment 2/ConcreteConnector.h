#pragma once

#include "CommandBase.h"
#include "Includes"

class ConcreteConnector : public CommandBase
{
public:
    Connector()
    {
        
    }
    
    virtual bool Execute() = 0;
};

class Semicolon : public ConcreteConnector
{
public:
    Semicolon(Base lhs, Base rhs) : lhs(lhs), rhs(rhs)
    {

    }
    
    bool Execute()
    {
        lhs->Execute();
        return rhs->Execute();
    }

private:
    Base lhs;
    Base rhs;
};

class And : public ConcreteConnector
{
public:
    And(Base lhs, Base rhs) : lhs(lhs), rhs(rhs)
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
    Base lhs;
    Base rhs;
};

class Or : public ConcreteConnector
{
public:
    Or(Base lhs, Base rhs) : lhs(lhs), rhs(rhs)
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
    Base lhs;
    Base rhs;
};