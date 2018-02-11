#pragma once

#include <vector>
#include <string>
#include <unistd.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <iostream>

class CommandReceiver
{
public:
    void Terminate() const;
    bool RunProgram(std::vector<std::string> &ArgumentList) const;
    bool ChangeDirectory() const;
};