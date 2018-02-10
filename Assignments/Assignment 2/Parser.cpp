#include "Parser.h"

Parser::Parser(std::string &Input) : Input(Input)
{
    
}

void Parser::ParseInput(std::vector<std::string> &InputVec, std::queue<std::string> &ConnectorList)
{
    //check if input is empty and for invalid input
    if (Input.empty())
    {
        return;
    }
    
    if (Input[0] == ';' || Input[0] == '&' || Input[0] == '|')
    {
        std::cout << "Shell: syntax error near unexpected token '" << Input[0] << "'\n"; 
        return;
    }

    //clear vector and queue
    InputList.clear();
    InputVec.clear();
	if (!ConnectorList.empty())
	{
		std::queue<std::string>().swap(ConnectorList);
	}
    //parse input
    // BalanceQuotations();
    // BalanceOperators();
    ParseQuotations();
    SpaceInput();
    // std::cout << "test" << std::endl;
    SplitInput();
    // std::cout << "test" << std::endl;
    ParseCommands(ConnectorList);
    // std::cout << "test" << std::endl;
    InputVec.swap(InputList);
    // std::cout << "test" << std::endl;
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
			break;
		}
	}
}

void Parser::ParseCommands(std::queue<std::string> &ConnectorList)
{
    for (auto i = 0U; i < InputList.size(); ++i)
    {
        if (InputList[i] == ";" || InputList[i] == "&&" || InputList[i] == "||")
        {
            if (SaveIndex.empty())
            {
                ConnectorList.emplace(InputList[i]);
            }
            else
            {
                if (!(SaveIndex[i].first[0] == '"' && SaveIndex[i].second == i))
                {
                    ConnectorList.emplace(InputList[i]);
                }
            }
        }
    }
}

void Parser::SplitInput()
{
    std::istringstream iss(Input);
    std::string Token;
    
    //splits input into tokens which get pushed into string a vector
    while (iss >> Token)
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
			for (auto j = i + 1; j < Input.size(); ++j)
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
    auto i = 0U;
    
    //save index of quotation mark to determine whether or not connector is within quotation marks
    while (iss >> temp)
    {
        for (auto j = 0U; j < temp.size(); ++j)
        {
            if (temp[j] == '"')
            {
                SaveIndex.emplace_back(std::make_pair(temp, i));
            }
        }
        
        ++i;
    }
    
    //erase quotation marks
    for (auto j = 0U; j < Input.size(); ++j)
    {
        if (Input[j] == '"')
        {
            Input.erase(j, 1);
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
            Input += temp;
            Input += "; echo ";
        }
        else
        {
            Input += "; echo ";
            Input += temp;
        }
    }
}

void Parser::BalanceOperators()
{
    
}