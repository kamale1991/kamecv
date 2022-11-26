/**
 * @file binary_image.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-06-04
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../include/binary_image.hpp"
#include <fstream>
#include <cassert>
#include <cmath>

namespace BI
{
	void Bitmap::show_head()
	{
		std::cout << "(width,height,ch) = (" << this->width << "," << this->height << "," << this->ch << std::endl;
	}

	void Bitmap::show_data()
	{
		int index = 0;
		for (int iy = 0; iy < height; iy++)
		{
			for (int ix = 0; ix < width; ix++)
			{
				for (int ic = 0; ic < ch; ic++)
				{
					std::cout << data[ic + ix * ch] << " ";
				}
				if (ix != width - 1)
				{
					std::cout << ", ";
				}
			}
			if (iy != height - 1)
			{
				std::cout << std::endl;
			}
		}
	}

	std::ostream &operator<<(std::ostream &os, const BytePixels &bp)
	{
		os << std::bitset<8>(bp.byte);
		return os;
	}

	void BinaryMapHeader::operator=(const BinaryMapHeader &rhs)
	{
		width = rhs.width;
		height = rhs.height;
		compression_type = rhs.compression_type;
		data_size_after_compression = rhs.data_size_after_compression;
	}

	bool BinaryMapHeader::operator==(const BinaryMapHeader &rhs)
	{
		return ((width == rhs.width) && (height == rhs.height) && (compression_type == rhs.compression_type) && (data_size_after_compression == rhs.data_size_after_compression));
	};

	void Bitmap::import_image(std::string filename){
			/*
			cv::Mat original_image;
			original_image = cv::imread(filename);
			this->width = original_image.size().width;
			this->height = original_image.size().height;
			this->ch = original_image.channels();
			this->data = (unsigned char *)original_image.data;
			*/
	};

	BinaryMap::BinaryMap(int16_t width, int16_t height, int8_t *data)
	{
		new_header();
		int nbyte = width * height;
		allocate(nbyte);
		int index;
		int countor = 0;
		int i_byte = 0;
		int i_bit = 0;
		for (int iy = 0; iy < height; iy++)
		{
			for (int ix = 0; ix < width; ix++)
			{
				std::cout << i_byte << " " << i_bit << " " << data[i_bit] << std::endl;
				data_[i_byte]->set(i_bit, data[i_bit]);
				countor++;
				i_bit++;
				if (countor == 8)
				{
					i_byte++;
					i_bit = 0;
				}
			}
		}
	};

	void BinaryMap::new_header()
	{
		header_ = new BinaryMapHeader;
		assert(header_);
	};

	void BinaryMap::allocate(int nbyte)
	{
		data_ = new BytePixels *[nbyte];
		assert(data_);
	};

	void BinaryMap::dump(std::string bim_filename)
	{
		std::ofstream ofs;
		ofs.open(bim_filename.c_str(), std::ios::trunc | std::ios::binary);
		if (!ofs)
		{
			return;
		};

		char *tmp;

		// Write header_
		tmp = (char *)&(header_->width);
		ofs.write(tmp, 2);
		tmp = (char *)&(header_->height);
		ofs.write(tmp, 2);
		tmp = (char *)&(header_->compression_type);
		ofs.write(tmp, 2);
		tmp = (char *)&(header_->data_size_after_compression);
		ofs.write(tmp, 2);

		// Write data
		int nbyte = number_of_byte();
		for (int i = 0; i < nbyte; i++)
		{
			tmp = (char *)&(data_[i]->byte);
			ofs.write(tmp, 1);
		};
		ofs.flush();
		ofs.close();
	};

	int BinaryMap::number_of_byte()
	{
		return std::ceil(header_->width * header_->height * 0.125);
	};

	void BinaryMap::shallow_copy(BinaryMapHeader *header_in, BytePixels **data_in)
	{
		this->header_ = header_in;
		this->data_ = data_in;
	};

	void BinaryMap::show_head()
	{
		std::cout << "width = " << this->header_->width << " ";
		std::cout << "height = " << this->header_->height << std::endl;
		std::cout << "compression type = " << this->header_->compression_type << " data_size_after_compression = " << this->header_->data_size_after_compression << std::endl;
	};

	void BinaryMap::show_data()
	{
		int nbyte = this->number_of_byte();
		for (int i = 0; i < nbyte; i++)
		{
			std::cout << *data_[i] << std::endl;
		}
	};

	void read_binary_map(std::string bim_filename, BinaryMap *out)
	{
		std::ifstream ifs(bim_filename.c_str(), std::ios::in | std::ios::binary);
		if (!ifs)
		{

			return;
		}
		char *tmp1 = new char[1];
		char *tmp2 = new char[2];
		char *tmp8 = new char[8];
		out->new_header();

		ifs.read(tmp8, 8);
		out->header_ = (BinaryMapHeader *)tmp8;

		int nbyte = out->number_of_byte();
		out->allocate(nbyte);
		for (int i = 0; i < nbyte; i++)
		{
			ifs.read(tmp1, 1);
			out->data_[i] = new BytePixels(static_cast<int8_t>(*tmp1));
		}

		ifs.close();
	};

	int8_t BinaryMap::get_bit(int ix, int iy)
	{
		int index = ix + header_->width * iy;
		int i_byte = std::floor(index * 0.125);
		int i_bit = index % 8;
		return data_[i_byte]->get(i_bit);
	};
};