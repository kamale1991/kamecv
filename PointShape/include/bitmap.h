/**
 * @file bitmap.h
 * @author kameta
 * @brief
 * @version 0.1
 * @date 2022-09-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef PS_BITMAP_H_
#define PS_BITMAP_H_

#include <iostream>
#include <stdint.h>
#include <vector>
namespace PointShape
{

	// position of (x,y) = (0,0) in the map coordinate
	enum Origin
	{
		LeftBottom = 0,
		LeftTop,
	};

	// mat[ic][ir] = {{1,2,3},{4,5,6}}
	// ColumnWise: 1,2,3,4,5,6
	// RowWise   : 1,4,2,5,3,6
	// if origing = left bottom,
	// ColumnWise denotes
	// 4 5 6
	// 1 2 3
	// RowWise dentes
	// 3 6
	// 2 5
	// 1 4

	enum MatrixTraversal
	{
		ColumnWise = 0,
		RowWise,
	};

	struct MapParam
	{
		uint64_t Nx1, Nx2;
	};

	template <class T>
	class Bitmap
	{
	protected:
		uint64_t Nx1_;
		uint64_t Nx2_;
		const uint64_t column_number_;
		const uint64_t row_number_;
		const Origin origin_;
		const MatrixTraversal traversal_;
		T *data_;

		uint64_t index_(uint64_t ix1, uint64_t ix2) const
		{
			return ix1 + Nx1_ * ix2;
		};

		void initialize_()
		{
			data_ = new T[column_number_ * row_number_];
			if (traversal_ == MatrixTraversal::ColumnWise)
			{
				Nx1_ = column_number_;
				Nx2_ = row_number_;
			}
			else if (traversal_ == MatrixTraversal::RowWise)
			{
				Nx1_ = row_number_;
				Nx2_ = column_number_;
			}
		}

	public:
		Bitmap(uint64_t column_number, uint64_t row_number, Origin origin, MatrixTraversal traversal) : column_number_(column_number), row_number_(row_number), origin_(origin), traversal_(traversal)
		{
			std::cout << traversal_ << std::endl;
			initialize_();
		};
		~Bitmap()
		{
			delete data_;
		};

		void set(uint64_t ix1, uint64_t ix2, T value)
		{
			data_[index_(ix1, ix2)] = value;
		}

		void set_2dim_vector(std::vector<std::vector<T>> vector)
		{
			if (Nx1_ != vector.size())
			{
				std::cout << "[Error] Invalid Nx1 size, Nx1=" << Nx1_ << " input=" << vector.size() << std::endl;
				return;
			}
			if (Nx2_ != vector.at(0).size())
			{
				std::cout << "[Error] Invalid Nx2 size, Nx2=" << Nx2_ << " input=" << vector.at(0).size() << std::endl;
				return;
			}
			for (uint64_t ix1 = 0; ix1 < Nx1_; ix1++)
			{
				for (uint64_t ix2 = 0; ix2 < Nx2_; ix2++)
				{
					set(ix1, ix2, vector.at(ix1).at(ix2));
				}
			}
		}

		T get(uint64_t ix1, uint64_t ix2) const
		{
			return data_[index_(ix1, ix2)];
		}

		void print() const
		{
			T val;
			if (column_number_ * row_number_ > 10000)
			{
				std::cout << "[Error] Too Huge Data to Print" << std::endl;
				return;
			}
			if (origin_ == Origin::LeftBottom)
			{
				for (uint64_t ix2 = Nx2_ - 1; ix2 != 0; ix2--)
				{
					for (uint64_t ix1 = 0; ix1 < Nx1_; ix1++)
					{
						val = get(ix1, ix2);
						std::cout << static_cast<int>(val) << ", ";
					}
					std::cout << std::endl;
				}
			}

			if (origin_ == Origin::LeftTop)
			{
				for (uint64_t ix2 = 0; ix2 < Nx2_; ix2++)
				{
					for (uint64_t ix1 = 0; ix1 < Nx1_; ix1++)
					{
						val = get(ix1, ix2);
						std::cout << static_cast<int>(val) << ", ";
					}
					std::cout << std::endl;
				}
			}
		}

		MapParam get_map_parameter() const
		{
			MapParam out = {Nx1_, Nx2_};
			return out;
		}
	};

} // namespace PointShape

#endif