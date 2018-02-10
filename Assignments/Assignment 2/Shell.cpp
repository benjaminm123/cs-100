#include "Shell.h"

Shell::Shell() : SingleCommandList(), ConnectorList()
{
    
}

void Shell::Run()
{
    bool Status = true;
    bool InfoAvailable = true;
    Receiver = new CommandReceiver();
    Invoker = new CommandInvoker();
    
    while (Status)
    {
        PrintPrompt(InfoAvailable);
        std::string Input;
        getline(std::cin, Input);
        Parser Parser(Input);
        Parser.ParseInput(InputVec, ConnectorList);
        Status = ExecuteCommands();
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

void Shell::CreateCommands()
{
    Command = new ConcreteSingleCommand(Receiver);

    for (auto i = 0U; i < InputVec.size(); ++i)
    {
        if (InputVec[i] == ";" || InputVec[i] == "&&" || InputVec[i] == "||")
        {
            SingleCommandList.emplace(Command);
            Command = new ConcreteSingleCommand(Receiver);
        }
        else
        {
            Command->Push(InputVec[i]);
        }
    }
    
    SingleCommandList.emplace(Command);
}

bool Shell::ExecuteCommands()
{
    CreateCommands();
    
    if (SingleCommandList.size() >= 2)
    {
        while (!SingleCommandList.empty())
        {
            BuildTree();
        }
        
        while (!CommandTree.empty())
        {
            Invoker->SetCommand(CommandTree.front());
            Invoker->RunCommand();
            CommandTree.pop_front();
        }
    }
    else
    {
        while (!SingleCommandList.empty())
        {
            Invoker->SetCommand(SingleCommandList.front());
            Invoker->RunCommand();
            SingleCommandList.pop();
        }
    }
    
    return true;
}

void Shell::BuildTree()
{
    //checks the command is joined by a "semicolon" operator
    if (ConnectorList.front() == ";")
    {
        //get lhs and rhs to push into command tree
        //save first command as lhs to pass into a "semicolon" object
        CommandBase *lhs = SingleCommandList.front();
        SingleCommandList.pop();
        //save second command as rhs to pass into a "semicolon" object
        CommandBase *rhs = SingleCommandList.front();
        SingleCommandList.pop();
        
        //create "semicolon" object and push into tree
        Connector = new Semicolon(lhs, rhs);
        CommandTree.emplace_front(Connector);
        
        //pop connector from queue
        ConnectorList.pop();
    }
    //checks if the command is joined by an "and" operator
    else if (ConnectorList.front() == "&&")
    {
        //if tree is empty get lhs from SingleCommandList
        if (CommandTree.empty())
        {
            //save first command as lhs to pass into an "and" object
            CommandBase *lhs = SingleCommandList.front();
            SingleCommandList.pop();
            //save first command as rhs to pass into an "and" object
            CommandBase *rhs = SingleCommandList.front();
            SingleCommandList.pop();
            
            //create "and" object and push into tree
            Connector = new And(lhs, rhs);
            CommandTree.emplace_front(Connector);
        }
        //if tree is not empty get lhs from back of tree
        else
        {
            CommandBase *lhs = CommandTree.back();
            CommandTree.pop_back();
            CommandBase *rhs = SingleCommandList.front();
            SingleCommandList.pop();
            
            Connector = new And(lhs, rhs);
            CommandTree.emplace_front(Connector);
        }
        
        //pop connector from queue
        ConnectorList.pop();
    }
    //checks if the command is joined by an "or" operator
    else if (ConnectorList.front() == "||")
    {
        //if tree is empty get lhs from SingleCommandList
        if (CommandTree.empty())
        {
            //save first command as lhs to pass into an "or" object
            CommandBase *lhs = SingleCommandList.front();
            SingleCommandList.pop();
            //save first command as lhs to pass into an "or" object
            CommandBase *rhs = SingleCommandList.front();
            SingleCommandList.pop();
            
            //create "or" object and push into tree    
            Connector = new Or(lhs, rhs);
            CommandTree.emplace_front(Connector);
        }
        //if tree is not empty get lhs from back of tree
        else
        {
            CommandBase *lhs = CommandTree.back();
            CommandTree.pop_back();
            CommandBase *rhs = SingleCommandList.front();
            SingleCommandList.pop();
            
            Connector = new Or(lhs, rhs);
            CommandTree.emplace_front(Connector);
        }
        
        //pop connector from queue
        ConnectorList.pop();
    }
}
