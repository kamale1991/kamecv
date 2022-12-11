#include <iostream>
#include <vector>
#include <string>

#include <boost/program_options.hpp>

// test()関数の定義
void test(const std::vector<int> &args)
{
	std::cout << "args[0]: " << args[0] << std::endl;
	std::cout << "args[1]: " << args[1] << std::endl;
	std::cout << "args[2]: " << args[2] << std::endl;
}

int main(int argc, char *argv[])
{
	// 引数を解析するためのオブジェクトを作成する
	boost::program_options::options_description desc("Options");
	desc.add_options()
			// "test"オプションを定義する
			("test", boost::program_options::value<std::vector<int>>()->multitoken(),
			 "test option");

	// コマンドライン引数を解析する
	boost::program_options::variables_map vm;
	boost::program_options::store(
			boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);

	// "test"オプションが与えられている場合は、test()を実行する
	if (vm.count("test"))
	{
		// "test"オプションの値を取得する
		std::vector<int> args = vm["test"].as<std::vector<int>>();

		// test()を実行する
		test(args);
	}

	return 0;
}