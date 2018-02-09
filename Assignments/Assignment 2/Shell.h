#include "ConcreteSingleCommand.h"

class Shell
{
public:
    void Run();
    void PrintPrompt(bool &InfoAvailable);
    
private:
    Receiver Executor;
    Base Command;
    Parser Parser;
};