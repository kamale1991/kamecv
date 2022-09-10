#include <iostream>
#include <chrono>

namespace Common
{
	class TPT
	{
	private:
		std::chrono::system_clock::time_point start_;
		const std::string proc_name_;

	public:
		TPT(std::string proc_name);
		~TPT(){};

		void stop();
	};
} // namespace Common
