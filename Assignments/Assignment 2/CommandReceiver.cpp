#include "rReceiver.h"

int CommandReceiver::Terminate()
{
   return 0;
}

bool CommandReceiver::RunProgram(std::vector<char *> &ArgumentList)
{
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

bool CommandReceiver::ChangeDirectory()
{
	bool ret = false;
	
	return ret;
}