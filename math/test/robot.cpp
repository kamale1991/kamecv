#include "../include/robot.h"
#include "gtest/gtest.h"
#include <cmath>
using namespace Bayse;

TEST(robot, transition)
{
	State golden(0.1, 0, 0);
	State test = state_transition(0.1, 0.0, 1.0, 0, 0, 0);
	EXPECT_EQ(golden, test);
};
