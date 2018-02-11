all: a.out

a.out: Main.o CommandReceiver.o Parser.o Shell.o CommandInvoker.h CommandReceiver.h ConcreteConnector.h Shell.h Parser.h ConcreteSingleCommand.h
	g++ -std=c++11 -Wall -Werror -o a.out Main.o CommandReceiver.o Parser.o Shell.o

Main.o: Main.cpp
	g++ -std=c++11 -Wall -Werror -c Main.cpp

CommandReceiver.o: CommandReceiver.cpp CommandReceiver.h
	g++ -std=c++11 -Wall -Werror -c CommandReceiver.cpp

Parser.o: Parser.cpp Parser.h
	g++ -std=c++11 -Wall -Werror -c Parser.cpp
	
Shell.o: Shell.cpp Shell.h
	g++ -std=c++11 -Wall -Werror -c Shell.cpp

clean:
	rm *.o a.out