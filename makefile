all: 
	g++ CLIGenie.cpp -o GenieCLI -Wall -Wextra -O3 -std=c++20
	g++ GenieInterface.cpp -o GenieInterface -Wall -Wextra -O3 -std=c++20

clean:
	$(RM) GenieCLI
	$(RM) GenieInterface
