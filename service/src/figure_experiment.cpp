#include "../include/figure_experiment.h"
#include <boost/program_options.hpp>

#include "../include/opencv_morphology.h"

void FigureExperiment::test(const std::vector<int> &args)
{
	std::cout << "args[0]: " << args[0] << std::endl;
	std::cout << "args[1]: " << args[1] << std::endl;
	std::cout << "args[2]: " << args[2] << std::endl;
}

void FigureExperiment::parse_command(int argc, char *argv[])
{
	// 引数を解析するためのオブジェクトを作成する
	boost::program_options::options_description desc("Options");
	desc.add_options()
			// "test"オプションを定義する
			("test", boost::program_options::value<std::vector<int>>()->multitoken(),
			 "test option")("morph_jpeg", boost::program_options::value<std::vector<std::string>>()->multitoken(), "read jpeg image file as grayscale figure run morphology process and dump as tiff format.\n args: <input_jpeg_path> <morph_type:0(erosion), 1(dilation), 2(opening)> <output_tiff_path>")("help,h", "show help message");

	// コマンドライン引数を解析する
	boost::program_options::variables_map vm;
	boost::program_options::store(
			boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);

	if (vm.count("help") || argc == 1)
	{
		std::cout << desc << std::endl;
		return;
	}

	if (vm.count("morph_jpeg"))
	{
		std::vector<std::string> args = vm["morph_jpeg"].as<std::vector<std::string>>();

		std::string input_jpeg_path = args[0];
		int morph_process_type = std::stoi(args[1]);
		std::string output_tiff_path = args[2];
		morph_jpeg(input_jpeg_path, morph_process_type, output_tiff_path);
	}

	// "test"オプションが与えられている場合は、test()を実行する
	if (vm.count("test"))
	{
		// "test"オプションの値を取得する
		std::vector<int> args = vm["test"].as<std::vector<int>>();

		// test()を実行する
		test(args);
		return;
	}

	std::cout << desc << std::endl;
	return;
};

void FigureExperiment::morph_jpeg(std::string input_jpeg_path, int morpha_type, std::string output_tiff_pat)
{
	// jpegを開いてbitmapに詰めなおす
	// bitmapのerosion機能を使用してerosion後のbitmapを作成する
	// bitmapをtiff形式で出力する
}