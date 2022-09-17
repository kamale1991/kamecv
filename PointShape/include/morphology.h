/**
 * @file morphology.h
 * @author kameta
 * @brief
 * @version 0.1
 * @date 2022-09-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef PS_MORPHOLOGY_H_
#define PS_MORPHOLOGY_H_

#include "bitmap.h"
#include <cmath>

namespace PointShape
{

	class GrayScaleImage : public Bitmap<uint8_t>
	{

	public:
		GrayScaleImage(uint64_t column_number, uint64_t row_number) : Bitmap<uint8_t>(column_number, row_number, Origin::LeftBottom, MatrixTraversal::ColumnWise){};
		~GrayScaleImage(){};
	};

	class StructureElement : public Bitmap<uint8_t>
	{
		uint64_t center_offset_x1_;
		uint64_t center_offset_x2_;

	public:
		StructureElement(uint64_t column_number, uint64_t row_number) : Bitmap<uint8_t>(column_number, row_number, Origin::LeftBottom, MatrixTraversal::ColumnWise)
		{
			center_offset_x1_ = std::floor(Nx1_ * 0.5);
			center_offset_x2_ = std::floor(Nx2_ * 0.5);
		};
		~StructureElement(){};

		void erosion(GrayScaleImage *operand);
		// void dilation(const GrayScaleImage *operand);
		// void opening(const GrayScaleImage *operand);
		// void closing(const GrayScaleImage *operand);
	};

} // namespace PointShape

#endif