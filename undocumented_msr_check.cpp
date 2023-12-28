#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

using namespace std;

int main()
{
	ifstream bash("msr-hex.txt");
	ifstream genie("output.txt");

	unordered_map<string, std::string> bash_map;
	unordered_map<string, std::string> genie_map;

	string line;
	while(getline(bash, line))
	{
		istringstream linestream(line);
		string token;
		vector<string> tokens;
		while(getline(linestream, token, ' '))
		{
			tokens.push_back(token);
		}
		assert(tokens.size() == 2);
		bash_map[tokens[0]] = tokens[1];
	}

	while(getline(genie, line))
	{
		if (line == "") continue;
		istringstream linestream(line);
		string token;
		vector<string> tokens;
		while(getline(linestream, token, ' '))
		{
			tokens.push_back(token);
		}
		assert(tokens.size() == 2);
		genie_map[tokens[0]] = tokens[1];
	}

	int count = 0;
	cout << "These MSRs are detected on octomore but not found in msr-genie output:";
	for(const auto &[msr, ret] : bash_map)
	{
		if(genie_map.find(msr) == genie_map.end())
		{
			count++;
			cout << "\n" << msr << " " << ret;
		}
	}
	cout << "\nnumber of undocumented MSRs found: " << count;

	return 0;
}
