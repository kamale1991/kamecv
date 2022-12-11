#include <iostream>
#include <vector>
#include <string>

#include <boost/program_options.hpp>

#include "service/include/figure_experiment.h"

int main(int argc, char *argv[])
{
	std::cout << "Hello, this is tool for your figure experiment." << std::endl;
	// 引数を解析するためのオブジェクトを作成する
	boost::program_options::options_description desc("Options");
	desc.add_options()
			// "test"オプションを定義する
			("test", boost::program_options::value<std::vector<int>>()->multitoken(),
			 "test option")("help,h", "show help message");

	// コマンドライン引数を解析する
	boost::program_options::variables_map vm;
	boost::program_options::store(
			boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);

	if (vm.count("help") || argc == 1)
	{
		std::cout << desc << std::endl;
		return 0;
	}

	// "test"オプションが与えられている場合は、test()を実行する
	if (vm.count("test"))
	{
		// "test"オプションの値を取得する
		std::vector<int> args = vm["test"].as<std::vector<int>>();

		// test()を実行する
		test(args);
		return 1;
	}

	std::cout << desc << std::endl;
	return 0;
}