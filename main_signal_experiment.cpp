#include <iostream>
#include <vector>
#include <string>

#include "service/include/signal_experiment.h"

int main(int argc, char *argv[])
{
	std::cout << "Hello, this is tool for your signal experiment." << std::endl;
	SignalExperiment sigexp;
	sigexp.parse_command(argc, argv);

	return 0;
}