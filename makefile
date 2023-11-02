all:
	g++ CLIGenie.cpp -o GenieCLI -Wall -Wextra -O3 -std=c++17
	g++ GenieInterface.cpp -o GenieInterface -Wall -Wextra -O3 -std=c++17

clean:
	$(RM) GenieCLI
	$(RM) GenieInterface

