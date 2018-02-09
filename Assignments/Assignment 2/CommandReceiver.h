#pragma once

#include "Includes.h"

class CommandReceiver
{
public:
    const int Terminate() const;
    bool RunProgram(std::vector<char *> &ArgumentList) const;
    bool ChangeDirectory() const;
};