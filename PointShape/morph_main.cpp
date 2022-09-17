/**
 * @file morph_main.cpp
 * @author kameta
 * @brief
 * @version 0.1
 * @date 2022-09-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <vector>
#include "include/morphology.h"
using namespace PointShape;
int main()
{
	std::cout << "<<Molphology Test>>" << std::endl;

	uint64_t ncol = 3;
	uint64_t nrow = 3;
	std::vector<std::vector<uint8_t>> se_vec = {
			{1, 2, 3},
			{4, 150, 6},
			{7, 8, 9}};
	StructureElement se(ncol, nrow);
	se.set_2dim_vector(se_vec);

	uint64_t ncol_img = 10;
	uint64_t nrow_img = 10;
	std::vector<std::vector<uint8_t>> img_vec(ncol_img, std::vector<uint8_t>(nrow_img));
	for (uint64_t iy = 0; iy < nrow_img; iy++)
	{
		for (uint64_t ix = 0; ix < ncol_img; ix++)
		{
			img_vec.at(ix).at(iy) = 0;
		}
	}
	for (uint64_t iy = 0; iy < nrow_img; iy++)
	{
		img_vec.at(iy).at(4) = 100;
		img_vec.at(iy).at(5) = 100;
		img_vec.at(iy).at(6) = 100;
	}
	GrayScaleImage img(ncol_img, nrow_img);
	img.set_2dim_vector(img_vec);
	se.print();
	img.print();
	se.erosion(&img);
	img.print();
}