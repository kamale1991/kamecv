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
#include "include/filter_gaussian.h"
using namespace PointShape;

void morp_test()
{
	std::cout << "<<Molphology Test>>" << std::endl;

	uint64_t ncol = 3;
	uint64_t nrow = 3;
	std::vector<std::vector<uint8_t>> se_vec = {
			{1, 2, 3},
			{4, 5, 6},
			{7, 8, 9}};
	StructureElement se(ncol, nrow);
	se.set_2dim_vector(se_vec);

	uint64_t ncol_img = 3;
	uint64_t nrow_img = 3;
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
		img_vec.at(iy).at(1) = 100;
	}
	GrayScaleImage img(ncol_img, nrow_img);
	GrayScaleImage out(ncol_img, nrow_img);
	img.set_2dim_vector(img_vec);
	se.print();
	img.print();
	se.erosion(&img, &out);
	img.print();
	out.print();
}

void gaussian_fileter_test()
{
	std::unique_ptr<GaussianFilter> gauss(new GaussianFilter());
	gauss->initialize();
	gauss->show();

	uint64_t ncol_img = 16;
	uint64_t nrow_img = 8;
	std::vector<std::vector<float>> img_vec(nrow_img, std::vector<float>(ncol_img));
	for (int iy = 0; iy < nrow_img; iy++)
	{
		for (int ix = 0; ix < ncol_img; ix++)
		{
			img_vec.at(iy).at(ix) = float(ix + ncol_img * iy);
		}
	}
	std::unique_ptr<fGrayImage> img(new fGrayImage(ncol_img, nrow_img));
	img->set_2dim_vector(img_vec);
	img->print();
	std::unique_ptr<fGrayImage> out = gauss->convolution(img);
	out->print();
};

int main()
{
	gaussian_fileter_test();
}