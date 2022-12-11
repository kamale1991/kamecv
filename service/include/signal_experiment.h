#include <iostream>
#include <vector>
#include <string>

// test()関数の定義

class SignalExperiment
{
public:
	SignalExperiment(){};
	~SignalExperiment(){};
	void parse_command(int argc, char *argv[]);
	void test(const std::vector<int> &args);
};
