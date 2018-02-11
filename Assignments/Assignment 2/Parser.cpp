#include "Parser.h"

Parser::Parser()
{

}

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

	//check if user tries to input a connector as the first word
	if (Input[0] == ';' || Input[0] == '&' || Input[0] == '|')
	{
		if (Input.size() > 1)
		{
			std::cout << "Shell: syntax error near unexpected token `" << Input[0] << Input[1] << "'\n";
		}
		else
		{
			std::cout << "Shell: syntax error near unexpected token `" << Input[0] << "'\n";
		}
		return false;
	}

	//clear vector and queue
	Clear();
	InputVec.clear();
	if (!ConnectorList.empty())
	{
		std::queue<std::string>().swap(ConnectorList);
	}

	//parse input
	ParseComments();
	BalanceQuotations();
	SpaceInput();
	ParseConnectors(ConnectorList);
	SplitInput();
	bool Ret = CheckSyntax();
	BalanceOperators(ConnectorList);
	InputVec.swap(InputList);

	return Ret;
}

void Parser::SpaceInput()
{
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

	auto j = 0U;
	//adds spaces to connectors
	for (auto i = 0U; i < Input.size(); ++i)
	{
		//check if connectors are in between two quotations
		//if they are do not add a space
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
	//initialize variables
	char temp = '\0';
	int QuotationCounter = 0;

	for (auto i = 0U; i < Input.size(); ++i)
	{
		//check if connector is in between two quotations
		if (QuotationCounter >= 2)
		{
			temp = '\0';
			QuotationCounter = 0;
		}
		if (Input[i] == '\"')
		{
			temp = Input[i];
			++QuotationCounter;
		}
		if ((Input[i] == ';') || (Input[i] == '&' && (Input[i + 1] == '&'))
							  || (Input[i] == '|' && Input[i + 1] == '|'))
		{
			//if connector is not in between two quotations 
			//create corresponding connector and push into queue
			if (temp != '\"')
			{
				std::string s;
				switch (Input[i])
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

bool Parser::ParseQuotations(std::string &TempInput, std::string &Input)
{
	bool HasConnector = false;
	//saves the position of quotation marks that will be erased
	for (auto i = 0U; i < Input.size(); ++i)
	{
		if (Input[i] == '\"')
		{
			if ((i > 0) && (Input[i - 1] != '\\'))
			{
				SavePosition.emplace_back(i);
			}
			else
			{
				SavePosition.emplace_back(i);
			}
		}
	}

	bool Saved = false;

	//saves the start and end range for quotation marks
	for (auto i = 0U; i < Input.size(); ++i)
	{
		if (Input[i] == '\"')
		{
			for (auto j = i + 1; j < Input.size(); ++j)
			{
				if (Input[j] == '\"')
				{
					SaveQuotation.emplace_back(i, j);
					Saved = true;
					i = j + 1;
					break;
				}
				else
				{
					Saved = false;
				}
			}

			if (!Saved)
			{
				SaveQuotation.emplace_back(i, UINT16_MAX);
			}
		}
	}

	auto j = 0U;

	//check if connector is in between two quotation marks
	for (auto i = 0U; i < Input.size(); ++i)
	{
		if ((Input[i] == '&') || (Input[i] == ';') || (Input[i] == '|'))
		{
			if (!SaveQuotation.empty())
			{
				if (!(i > SaveQuotation[j].first && i < SaveQuotation[j].second))
				{
					++j;
				}
				else
				{
					HasConnector = true;
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
	//erase backlashes
	for (auto i = 0U; i < Input.size(); ++i)
	{
		if (Input[i] == '\\')
		{
			Input.erase(i, 1);
		}
	}
	
	//assign tempinput to input
	TempInput = Input;

	return HasConnector;
}

void Parser::BalanceQuotations()
{
	//balances unclosed '"', '&&', and '||'
	int QuoteCount = 0;

	//count number of quotes in input
	for (auto i = 0U; i < Input.size(); ++i)
	{
		if ((i > 0) && (Input[i] == '\"'))
		{
			if (Input[i - 1] != '\\')
			{
				++QuoteCount;
			}
		}
		else if (!i && Input[i] == '\"')
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
		}
		else
		{
			Input += " " + temp;
		}
	}
}

void Parser::BalanceOperators(std::queue<std::string> &ConnectorList)
{
	std::string Temp;
	bool HasNext = false;

	while (!HasNext)
	{
		for (auto i = 0U; i < InputList.size(); ++i)
		{
			Parser parse;
			bool HasConnector = parse.ParseQuotations(Temp, InputList[i]);
			InputList[i] = Temp;

			if ((InputList[i] == "||" || InputList[i] == "&&") && !HasConnector)
			{
				if ((i + 1) >= InputList.size())
				{
					HasNext = false;
				}
				else
				{
					HasNext = true;
				}
			}
			else
			{
				HasNext = true;
			}
		}
		
		if (!HasNext)
		{
			std::cout << "> ";
			getline(std::cin, Input);
			ParseComments();
			BalanceQuotations();
			SpaceInput();
			CheckSyntax();
			ParseConnectors(ConnectorList);
			std::istringstream iss(Input);
			std::string Token;
			while (iss >> Token)
			{
				InputList.emplace_back(Token);
			}
		}
	}
}

bool Parser::CheckSyntax()
{
	for (auto i = 0U; i < InputList.size() - 1; ++i)
	{
		if (InputList[i] == ";" || InputList[i] == "||" || InputList[i] == "&&")
		{
			if (InputList[i + 1] == ";" || InputList[i + 1] == "||" || InputList[i + 1] == "&&")
			{
				std::cout << "Shell: syntax error near unexpected token `" << InputList[i] << "'\n";
				return false;
			}
			else if (InputList[i - 1] == ";" || InputList[i - 1] == "||" || InputList[i - 1] == "&&")
			{
				std::cout << "Shell: syntax error near unexpected token `" << InputList[i] << "'\n";
				return false;
			}
		}
	}

	return true;
}

void Parser::Clear()
{
	InputList.clear();
	SavePosition.clear();
	SaveQuotation.clear();
}

void Parser::ParseComments()
{
	for (auto i = 0U; i < Input.size(); ++i)
	{
		if (Input[i] == '#')
		{
			Input.erase(Input.begin() + i, Input.end());
			break;
		}
	}
}