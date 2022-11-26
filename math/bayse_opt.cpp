#include "include/robot.h"
#include <iostream>
using namespace Bayse;
int main()
{

	State test = state_transition(0.1, 0.0, 1.0, 0, 0, 0);
	std::cout << test << std::endl;
}