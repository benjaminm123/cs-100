#pragma once

#include "Includes.h"

class CommandReceiver
{
public:
    void Terminate() const;
    bool RunProgram(const std::vector<char *> &ArgumentList) const;
    bool ChangeDirectory() const;
};