/**
 * @file filter_gaussian.h
 * @author kameta
 * @brief
 * @version 0.1
 * @date 2022-11-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <cmath>
#include <string>
#include <iostream>
#include <sstream>
#include <memory>

#include "mkl_vsl.h"
#include "mkl.h"

namespace PointShape
{

	class fGrayImage : public Bitmap<float>
	{
	public:
		fGrayImage(uint64_t column_number, uint64_t row_number) : Bitmap<float>(column_number, row_number, Origin::LeftBottom, MatrixTraversal::ColumnWise){};
		~fGrayImage(){};
	};

	class GaussianFilter
	{
		float *kernel;

	public:
		GaussianFilter(){};
		~GaussianFilter(){};

		// column -> row
		void initialize()
		{
			int nkmesh = 25;
			kernel = new float[nkmesh];
			kernel[0] = 1;
			kernel[4] = 1;
			kernel[20] = 1;
			kernel[24] = 1;
			kernel[1] = 4;
			kernel[3] = 4;
			kernel[5] = 4;
			kernel[9] = 4;
			kernel[19] = 4;
			kernel[15] = 4;
			kernel[21] = 4;
			kernel[23] = 4;
			kernel[2] = 7;
			kernel[10] = 7;
			kernel[14] = 7;
			kernel[22] = 7;
			kernel[6] = 16;
			kernel[8] = 16;
			kernel[16] = 16;
			kernel[18] = 16;
			kernel[7] = 26;
			kernel[11] = 26;
			kernel[13] = 26;
			kernel[17] = 26;
			kernel[12] = 41;
			for (int i = 0; i < nkmesh; i++)
			{
				kernel[i] /= 273.0;
			}
		};

		std::unique_ptr<fGrayImage> convolution(std::unique_ptr<fGrayImage> &target)
		{
			// Matrix Dimension
			MKL_INT target_dim[2] = {(MKL_INT)target->get_map_parameter().Nx1, (MKL_INT)target->get_map_parameter().Nx2};
			MKL_INT kernel_dim[2] = {5, 5};
			MKL_INT out_dim[2] = {target_dim[0] + kernel_dim[0] - 1, target_dim[1] + kernel_dim[1] - 1};
			MKL_INT ystart[2] = {0, 0};
			MKL_INT target_stride[2] = {target_dim[1], 1};
			MKL_INT kernel_stride[2] = {kernel_dim[1], 1};
			MKL_INT out_stride[2] = {out_dim[1], 1};

			// MKL setting
			int status;
			VSLConvTaskPtr task;
			status = vslsConvNewTask(&task, VSL_CONV_MODE_FFT, 2, target_dim, kernel_dim, out_dim);
			if (status != VSL_STATUS_OK)
			{
				std::cerr << "[Error] GaussianKernel::convolution failed to set new task" << std::endl;
			}
			status = vslConvSetStart(task, ystart);
			if (status != VSL_STATUS_OK)
			{
				std::cerr << "[Error] GaussianKernel::convoluiton failed to set start" << std::endl;
			}

			std::unique_ptr<fGrayImage> out(new fGrayImage(out_dim[0], out_dim[1]));

			status = vslsConvExec(task, target->get_ptr(), target_stride, kernel, kernel_stride, out->get_ptr(), out_stride);
			if (status != VSL_STATUS_OK)
			{
				std::cerr << "[Error] GaussianKernel::convolution failed to execute convolution" << std::endl;
			}
			status = vslConvDeleteTask(&task);
			if (status != VSL_STATUS_OK)
			{
				std::cerr << "[Error] GaussianKernel::convolution failed to delete task" << std::endl;
			}
			return out;
		};

		void show()
		{
			std::ostringstream os;
			os << "[GaussianKernel]" << std::endl;
			for (int iy = 4; iy >= 0; iy--)
			{
				for (int ix = 0; ix < 5; ix++)
				{
					os << kernel[ix + 5 * iy];
					if (ix != 4)
					{
						os << ", ";
					}
				}
				os << std::endl;
			}
			std::cout << os.str();
		}
	};

} // namespace PointShape
