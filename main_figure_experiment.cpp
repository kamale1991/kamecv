#include <iostream>
#include <vector>
#include <string>

#include "service/include/figure_experiment.h"

int main(int argc, char *argv[])
{
	std::cout << "Hello, this is tool for your figure experiment." << std::endl;
	FigureExperiment figexp;
	figexp.parse_command(argc, argv);

	return 0;
}