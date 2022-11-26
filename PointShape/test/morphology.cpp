#include "gtest/gtest.h"
#include "../include/morphology.h"

using namespace PointShape;

TEST(erosion, test1)
{
	uint64_t ncol_img = 4;
	uint64_t nrow_img = 5;
	std::vector<std::vector<uint8_t>> img_vec = {
			{0, 1, 2, 3},
			{4, 5, 6, 7},
			{8, 9, 10, 11},
			{12, 13, 14, 15},
			{16, 17, 18, 19}};
	uint64_t ncol = 2;
	uint64_t nrow = 2;
	std::vector<std::vector<uint8_t>> se_vec = {
			{1, 1},
			{1, 1}};
	StructureElement se(ncol, nrow);
	se.set_2dim_vector(se_vec);

	GrayScaleImage img(ncol_img, nrow_img);
	GrayScaleImage out(ncol_img, nrow_img);
	img.set_2dim_vector(img_vec);
	se.print();
	img.print();
	se.erosion(&img, &out);
	img.print();
	out.print();
};
