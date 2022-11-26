#include "../include/tpt.h"

using namespace std::chrono;

namespace Common
{
	TPT::TPT(std::string proc_name) : proc_name_(proc_name)
	{
		start_ = system_clock::now();
	};

	void TPT::stop()
	{

		system_clock::time_point current_time = system_clock::now();
		nanoseconds nsec = duration_cast<nanoseconds>(current_time - start_);
		microseconds usec = duration_cast<microseconds>(nsec);
		milliseconds msec = duration_cast<milliseconds>(nsec);
		std::cout << "[TPT] Proc: " << proc_name_ << " " << nsec.count() << " [nsec] " << usec.count() << " [usec] " << msec.count() << " [msec]" << std::endl;
	}
} // namespace Common
