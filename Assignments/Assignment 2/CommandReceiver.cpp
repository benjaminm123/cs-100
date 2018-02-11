#include "CommandReceiver.h"

void CommandReceiver::Terminate() const
{
   exit(EXIT_SUCCESS);
}

bool CommandReceiver::RunProgram(std::vector<std::string> &ArgumentList) const
{
	if (ArgumentList[0] == "exit")
	{
		Terminate();
	}
    
	std::vector<char *> Args;
	
	for (auto i = 0U; i < ArgumentList.size(); ++i)
	{
		Args.emplace_back(const_cast<char *>(ArgumentList[i].c_str()));
	}
	
	Args.emplace_back(nullptr);
    
    pid_t pid = fork();
	
	if (pid > 0)
	{
        int Status;
        
        if (waitpid(pid, &Status, 0) == -1) 
        {       
            perror("wait");
        }
        
        if (WEXITSTATUS(Status) != 0) 
        {
            return false;
        }
	}
	else if (pid == 0)
	{
		if (execvp(Args[0], Args.data()) == -1)
		{
			std::cout << "Shell: " << Args[0] << ": " << "command not found" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	else if (pid < 0)
	{
		perror("fork");
		return false;
	}
	
	return true;
}

bool CommandReceiver::ChangeDirectory() const
{
	bool ret = false;
	
	return ret;
}