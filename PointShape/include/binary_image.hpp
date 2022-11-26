/**
 * @file binary_image.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-06-03
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>
#include <iostream>
#include <bitset>
#include <string>

//#include <opencv2/opencv.hpp>

#ifndef BI_BINARY_IMAGE_H_
#define BI_BINARY_IMAGE_H_

//#include "jpeglib.h"

namespace BI
{

	struct BytePixels
	{
		BytePixels()
		{
			byte = 0b00000000;
		};
		BytePixels(int8_t input_byte) : byte(input_byte){};
		int8_t byte;

		void set(int8_t index, int8_t sw)
		{
			if (sw == 1)
			{
				this->byte |= (1 << index);
			}
			if (sw == 0)
			{
				this->byte &= (~(1 << index));
			}
		};

		void zero()
		{
			byte = 0b00000000;
		}

		void one()
		{
			byte = 0b11111111;
		}

		int8_t get(int8_t index)
		{
			return this->byte & (1 << index);
		};

		bool operator==(const BytePixels &rhs)
		{
			return byte == rhs.byte;
		}
		friend std::ostream &operator<<(std::ostream &os, const BytePixels &bp);
	};

	class Bitmap
	{
	public:
		unsigned char *data;
		uint32_t width;
		uint32_t height;
		uint32_t ch; // 画像の色数: RGBの時には3, グレーでは1
		void show_head();
		void show_data();
		void import_image(std::string filename);
	};

	struct BinaryMapHeader
	{
		int16_t width;
		int16_t height;
		// 0: No compression
		int16_t compression_type;
		int16_t data_size_after_compression;
		BinaryMapHeader(){};
		BinaryMapHeader(int16_t w, int16_t h, int16_t comp_type, int16_t size) : width(w), height(h), compression_type(comp_type), data_size_after_compression(size){};
		BinaryMapHeader(BinaryMapHeader *header)
		{
			this->width = header->width;
			this->height = header->height;
			this->compression_type = header->compression_type;
			this->data_size_after_compression = header->data_size_after_compression;
		};
		void operator=(const BinaryMapHeader &rhs);
		bool operator==(const BinaryMapHeader &rhs);
	};

	class BinaryMap
	{
	public:
		// Header
		BinaryMapHeader *header_;
		// Data
		BytePixels **data_;

		// orientation: 0
		int orientation = 0;

		BinaryMap(){};
		BinaryMap(BinaryMapHeader *header_in, BytePixels **data_in) : header_(header_in), data_(data_in){};
		// BinaryMap(cv::Mat *cv_mat){};
		BinaryMap(Bitmap *bitmap){};
		BinaryMap(int16_t width, int16_t height, int8_t *data);

		void new_header();
		void allocate(int nbyte);
		void dump(std::string bim_filename);
		int number_of_byte();
		void shallow_copy(BinaryMapHeader *header_in, BytePixels **data_in);
		void show_head();
		void show_data();

		int8_t get_bit(int ix, int iy);
	};
	void read_binary_map(std::string bim_filename, BinaryMap *out);
};

#endif