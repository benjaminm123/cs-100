#include "Shell.h"

void Shell::Run()
{
    bool Status = true;
    bool InfoAvailable = true;
    Executor = std::make_shared<Receiver>();
    
    while (Status)
    {
        PrintPrompt(InfoAvailable);
        Status = Parser.ParseInput(Command, Executor);
    }
}

void Shell::PrintPrompt(bool &InfoAvailable)
{
    if (InfoAvailable)
    {
        char* UserName = getlogin();
        
        if (UserName == NULL)
        {
            perror("getlogin() failed");
            InfoAvailable = false;
        }
    
        char HostName[256] = "/0";
        gethostname(HostName, 256); 
        
        if (HostName == NULL)
        {
            perror("gethostname() failed");
            InfoAvailable = false;
        }
        
        if (InfoAvailable)
        {
            std::cout << "[" << UserName << "@" << HostName << "]";
        }
    }
    
    std::cout << "$ ";
}