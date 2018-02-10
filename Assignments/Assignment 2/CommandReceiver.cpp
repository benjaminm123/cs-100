#include "CommandReceiver.h"

void CommandReceiver::Terminate() const
{
   exit(EXIT_SUCCESS);
}

bool CommandReceiver::RunProgram(const std::vector<char *> &ArgumentList) const
{
    if (!strcmp(ArgumentList[0], "exit"))
    {
    	Terminate();
    }
    
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
		if (execvp(ArgumentList[0], ArgumentList.data()) == -1)
		{
			std::cout << "Shell: " << ArgumentList[0] << ": " << "command not found" << std::endl;
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