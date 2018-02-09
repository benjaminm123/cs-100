#include "Parser.h"

Parser::Parser(std::string &Input, Receiver Executor) : Input(Input), Executor(Executor)
{
    
}

bool Parser::ParseInput()
{
    //check if input is empty and for invalid input
    if (Input[0] == ';' || Input[0] == '&' || Input[0] == '|' || Input.empty())
    {
        return false;
    }
    
    //clear vector and queue
    InputList.clear();
    std::queue<std::string>().swap(Connectors);
    
    //parse input
    BalanceInput();
    ParseQuotations();
    SpaceInput();
    SplitInput();
    ParseCommands();
    
    return true;
}

void Parser::SpaceInput()
{
	//adds spaces to connectors
	for (auto i = 0U; i < Input.size(); ++i)
	{
		//check for connectors
		switch (Input[i])
		{
		case ';':
			(Input[i + 1] != ' ') ? Input.insert(i + 1, " ") : Input;
			(Input[i - 1] != ' ') ? Input.insert(i, " ") : Input;
			break;

		case '&':
			(Input[i - 1] != ' ' && Input[i - 1] != '&') ? Input.insert(i, " ") : Input;
			(Input[i + 1] != ' ' && Input[i + 1] != '&') ? Input.insert(i + 1, " ") : Input;
			break;

		case '|':
			(Input[i - 1] != ' ' && Input[i - 1] != '|') ? Input.insert(i, " ") : Input;
			(Input[i + 1] != ' ' && Input[i + 1] != '|') ? Input.insert(i + 1, " ") : Input;
			break;

		default:
		    std::cout << "Error: Unknown Type\n"
			break;
		}
	}
}

void Parser::BuildTree()
{
    //checks the command is joined by a "semicolon" operator
    if (Connectors.front() == ";")
    {
        //get lhs and rhs to push into command tree
        while (!SingleCommandList.empty())
        {
            //save first command as lhs to pass into a "semicolon" object
            Base lhs = SingleCommandList.front();
            SingleCommandList.pop();
            //save second command as rhs to pass into a "semicolon" object
            Base rhs = SingleCommandList.front();
            SingleCommandList.pop();
            
            //create "semicolon" object and push into tree
            Connector = std::make_shared<Semicolon>(lhs, rhs);
            CommandTree.emplace(Semicolon);
        }
        
        //pop connector from queue
        Connectors.pop();
    }
    //checks if the command is joined by an "and" operator
    else if (Connectors.front() == "&&")
    {
        //if tree is empty get lhs from SingleCommandList
        if (CommandTree.empty())
        {
            while (!SingleCommandList.empty())
            {
                //save first command as lhs to pass into an "and" object
                Base lhs = SingleCommandList.front();
                SingleCommandList.pop();
                //save first command as rhs to pass into an "and" object
                Base rhs = SingleCommandList.front();
                SingleCommandList.pop();
                
                //create "and" object and push into tree
                Connector = std::make_shared<And>(lhs, rhs);
                CommandTree.emplace_front(And);
            }
        }
        //if tree is not empty get lhs from back of tree
        else
        {
            Base lhs = CommandTree.back();
            CommandTree.pop_back();
            Base rhs = SingleCommandList.front();
            SingleCommandList.pop();
            
            Connector = std::make_shared<And>(lhs, rhs);
            CommandTree.emplace_front(And);
        }
        
        //pop connector from queue
        Connectors.pop();
    }
    //checks if the command is joined by an "or" operator
    else if (Connectors.front() == "||")
    {
        //if tree is empty get lhs from SingleCommandList
        if (CommandTree.empty())
        {
            while (!SingleCommandList.empty())
            {
                //save first command as lhs to pass into an "or" object
                Base lhs = SingleCommandList.front();
                SingleCommandList.pop();
                //save first command as lhs to pass into an "or" object
                Base rhs = SingleCommandList.front();
                SingleCommandList.pop();
                
                //create "or" object and push into tree    
                Connector = std::make_shared<Or>(lhs, rhs);
                CommandTree.emplace_front(Or);
            }
        }
        //if tree is not empty get lhs from back of tree
        else
        {
            Base lhs = CommandTree.back();
            CommandTree.pop_back();
            Base rhs = SingleCommandList.front();
            SingleCommandList.pop();
            
            Connector = std::make_shared<Or>(lhs, rhs);
            CommandTree.emplace_front(Or);
        }
        
        //pop connector from queue
        Connectors.pop();
    }
}

void Parser::ParseCommands(Base Command, Receiver Executor)
{
    unsigned int i, j;
    
    for (i = 0, j = 0; i < InputList.size(); ++i, ++j)
    {
        //checks if connectors are within quotation marks
        if ((i == ";") || (i == "&&") || (i == "||"))
        {
            if (!((SaveIndex[j].first[0] == '"') && (SaveIndex[j].second == i)))
            {
                Connectors.emplace(InputList[i]);
            }
        }
        else
        {
            //creates a single command pointer and pushes it into a queue for tree building
            Command = std::make_shared<ConcreteSingleCommand>(Executor);
            SingleCommandList.emplace(Command);
            
            //builds a tree from single commands
            BuildTree();
        }
    }
}

void Parser::SplitInput()
{
    istringstream iss(Input);
    std::string Token;
    
    //splits input into tokens which get pushed into string a vector
    while (iss >> token)
    {
        InputList.emplace_back(Token);
    }
}

void Parser::ParseQuotations()
{
    //saves the position of quotation marks that will be erased
    for (auto i = 0U; i < Input.size(); ++i)
    {
        if (Input[i] == '"')
		{
			for (int j = i + 1; j < Input.size(); ++j)
			{
				if (Input[j] == '"')
				{
					SavePosition.emplace_back(std::make_pair(i, j));
					i = j + 1;
					break;
				}
			}
		}
    }
    
    std::istringstream iss(Input);
    std::string temp;
    unsigned int i = 0;
    
    //save index of quotation mark to determine whether or not connector is within quotation marks
    while (iss >> temp)
    {
        SaveIndex.emplace_back(std::make_pair(temp, i));
        ++i;
    }
    
    //erase quotation marks
    for (auto i = 0U; i < Input.size(); ++i)
    {
        if (Input[i] == '"')
        {
            Input.erase(i, 1);
        }
    }
}

void Parser::BalanceQuotations()
{
    //balances unclosed '"', '&&', and '||'
    int QuoteCount = 0;
    
    for (auto i = 0U; i < Input.size(); ++i)
    {
        if (Input[i] == '"' && Input[i - 1] != '\\')
        {
            ++QuoteCount;
        }
    }
    
    while ((QuoteCount % 2))
    {
        std::cout << "> ";
        
        std::string temp;
        getline(std::cin, temp);
        
        for (auto i = 0U; i < temp.size(); ++i)
        {
            if (temp[i] == '"')
            {
                ++QuoteCount;
            }
        }
        
        if (temp == "\"")
        {
            input += temp;
            input += "; echo ";
        }
        else
        {
            input += "; echo ";
            input += temp;
        }
    }
}

void Parser::BalanceOperators()
{
    
}