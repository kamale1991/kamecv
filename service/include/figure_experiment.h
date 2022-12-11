#include <iostream>
#include <vector>
#include <string>

class FigureExperiment
{
public:
	FigureExperiment(){};
	~FigureExperiment(){};
	// test()関数の定義
	void test(const std::vector<int> &args);

	void parse_command(int argc, char *argv[]);

	void morph_jpeg(std::string input_jpeg_path, int morpha_type, std::string output_tiff_pat);
};
