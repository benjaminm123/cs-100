#pragma once

#include <cstring>
#include <iostream>
#include <memory>
#include <unistd.h>
#include <vector>
#include <queue>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <sstream>
#include <cstdlib>
#include <string>
#include <stack
#include <deque>

typedef std::shared_ptr<CommandBase> Base;
typedef std::shared_ptr<CommandReceiver> Receiver
typedef std::shared_ptr<CommandInvoker> Invoker
typedef std::shared_ptr<ConcreteSingleCommand> SingleCommand;
typedef std::shared_ptr<ConcreteConnector> Connector;