#include "Parser.h"

Parser::Parser(std::string &Input) : Input(Input)
{
    
}

bool Parser::ParseInput(std::vector<std::string> &InputVec, std::queue<std::string> &ConnectorList)
{
    //check if input is empty and for invalid input
    if (Input.empty())
    {
        return false;
    }
    
    if (Input[0] == ';' || Input[0] == '&' || Input[0] == '|')
    {
        std::cout << "Shell: syntax error\n"; 
        return false;
    }

    //clear vector and queue
    InputList.clear();
    InputVec.clear();
	if (!ConnectorList.empty())
	{
		std::queue<std::string>().swap(ConnectorList);
	}
    
    //parse input
    BalanceQuotations();
    SaveInput = Input;
    ParseQuotations();
    // std::cout << "test" << std::endl;
	SpaceInput();
// 	std::cout << "test" << std::endl;
	ParseConnectors(ConnectorList);
// 	std::cout << "test" << std::endl;
    // BalanceOperators();
    SplitInput();
    // std::cout << "test" << std::endl;
    bool ret = CheckSyntax();
    
	InputVec.swap(InputList);

    return ret;
}

void Parser::SpaceInput()
{
	auto j = 0U;
	//adds spaces to connectors
	for (auto i = 0U; i < Input.size(); ++i)
	{
		//check for connectors
		switch (Input[i])
		{
		case ';':
			if (!SaveQuotation.empty() && j < SaveQuotation.size())
			{
				if (!(i > SaveQuotation[j].first && i < SaveQuotation[j].second))
				{
					(Input[i + 1] != ' ') ? Input.insert(i + 1, " ") : Input;
					(Input[i - 1] != ' ') ? Input.insert(i, " ") : Input;
					++i;
					++j;
				}
			}
			else
			{
				(Input[i + 1] != ' ') ? Input.insert(i + 1, " ") : Input;
				(Input[i - 1] != ' ') ? Input.insert(i, " ") : Input;
				++i;
			}
			break;

		case '&':
			if (!SaveQuotation.empty() && j < SaveQuotation.size())
			{
				if (!(i > SaveQuotation[j].first && i < SaveQuotation[j].second))
				{
					(Input[i - 1] != ' ' && Input[i - 1] != '&') ? Input.insert(i, " ") : Input;
					(Input[i + 1] != ' ' && Input[i + 1] != '&') ? Input.insert(i + 1, " ") : Input;
					++j;
					++i;
				}
			}
			else
			{
				(Input[i - 1] != ' ' && Input[i - 1] != '&') ? Input.insert(i, " ") : Input;
				(Input[i + 1] != ' ' && Input[i + 1] != '&') ? Input.insert(i + 1, " ") : Input;
				++i;
			}
			break;

		case '|':
			if (!SaveQuotation.empty() && j < SaveQuotation.size())
			{
				if (!(i > SaveQuotation[j].first && i < SaveQuotation[j].second))
				{
					(Input[i - 1] != ' ' && Input[i - 1] != '|') ? Input.insert(i, " ") : Input;
					(Input[i + 1] != ' ' && Input[i + 1] != '|') ? Input.insert(i + 1, " ") : Input;
					++i;
					++j;
				}
			}
			else
			{
				(Input[i - 1] != ' ' && Input[i - 1] != '|') ? Input.insert(i, " ") : Input;
				(Input[i + 1] != ' ' && Input[i + 1] != '|') ? Input.insert(i + 1, " ") : Input;
				++i;
			}
			break;

		default:
			break;
		}
	}
}

void Parser::ParseConnectors(std::queue<std::string> &ConnectorList)
{
	char temp = '\0';
	int QuotationCounter = 0;

	for (auto i = 0U; i < SaveInput.size(); ++i)
	{
		if (QuotationCounter >= 2)
		{
			temp = '\0';
			QuotationCounter = 0;
		}
		if (SaveInput[i] == '\"')
		{
			temp = SaveInput[i];
			++QuotationCounter;
		}
		if (SaveInput[i] == ';' || SaveInput[i] == '&' && SaveInput[i + 1] == '&' 
								|| SaveInput[i] == '|' && SaveInput[i + 1] == '|')
		{
			if (temp != '\"')
			{
				std::string s;
				switch (SaveInput[i])
				{
				case ';':
					s = ";";
					ConnectorList.emplace(s);
					break;
				
				case '&':
					s = "&&";
					ConnectorList.emplace(s);
					break;
				
				case '|':
					s = "||";
					ConnectorList.emplace(s);
					break;
				
				default:
					break;
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
        if (Input[i] == '\"' && Input[i - 1] != '\\')
		{
            SavePosition.emplace_back(i);
		}
    }
    
    //save quotation index
	for (auto i = 0U; i < Input.size(); ++i)
	{
		if (Input[i] == '\"')
		{
			for (auto j = i + 1; j < Input.size(); ++j)
			{
				if (Input[j] == '\"')
				{
					SaveQuotation.emplace_back(i, j);
					i = j + 1;
					break;
				}
			}
		}
	}
    
    //erase quotation marks
	for (auto i = 0U; i < SavePosition.size(); ++i)
	{
		Input.erase(SavePosition[i], 1);
		if (i < SavePosition.size() - 1)
		{
			SavePosition[i + 1] = SavePosition[i + 1] - (1 + i);
		}
	}

	for (auto i = 0U; i < Input.size(); ++i)
	{
		if (Input[i] == '\\')
		{
			Input.erase(i, 1);
		}
	}
}

void Parser::BalanceQuotations()
{
    //balances unclosed '"', '&&', and '||'
    int QuoteCount = 0;
    
    //count number of quotes in input
    for (auto i = 0U; i < Input.size(); ++i)
    {
        if (Input[i] == '"' && Input[i - 1] != '\\')
        {
            ++QuoteCount;
        }
    }
    
    //check for even quote count
    while ((QuoteCount % 2))
    {
        std::cout << "> ";
        
        //if quote count is not even ask for input
        std::string temp;
        getline(std::cin, temp);
        
        for (auto i = 0U; i < temp.size(); ++i)
        {
            if (temp[i] == '"')
            {
                ++QuoteCount;
            }
        }
        
        //add newline after input
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

bool Parser::CheckSyntax()
{
	for (auto i = 0U; i < InputList.size(); ++i)
	{
		if (InputList[i] == ";" || InputList[i] == "||" || InputList[i] == "&&")
		{
			if (InputList[i + 1] == ";" || InputList[i + 1] == "||" || InputList[i + 1] == "&&")
			{
				std::cout << "Shell: syntax error\n"; 
				return false;
			}
			else if (InputList[i - 1] == ";" || InputList[i - 1] == "||" || InputList[i - 1] == "&&")
			{
				std::cout << "Shell: syntax error\n"; 
				return false;
			}
		}
	}
	
	return true;
}