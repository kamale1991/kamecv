/**
 * @file one_bit_tiff.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-06-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../include/one_bit_tiff.h"

namespace BI
{

	TiffHeader TiffHeader::get_header_little_endien()
	{
		TiffHeader head('I', 'I');
		return head;
	}

	TiffHeader TiffHeader::get_header_big_endien()
	{
		TiffHeader head('M', 'M');
		return head;
	}

	std::ostream &operator<<(std::ostream &os, const TiffHeader &head)
	{
		os << "byte_order: " << head.byte_order_0_ << head.byte_order_1_ << std::endl;
		os << "magic_number: " << head.magic_number_ << std::endl;
		os << "offset: " << head.offset_ << std::endl;
		return os;
	};

	constexpr const int TiffDataType::value() const
	{
		return type_;
	}

	constexpr const char *TiffDataType::name() const
	{
		if (type_ == 1)
			return "Byte";
		if (type_ == 2)
			return "Ascii";
		if (type_ == 3)
			return "Short";
		if (type_ == 4)
			return "Long";
		if (type_ == 5)
			return "Rational";
		if (type_ == 6)
			return "SByte";
		if (type_ == 7)
			return "Undefined";
		if (type_ == 8)
			return "SShort";
		if (type_ == 9)
			return "SLong";
		if (type_ == 10)
			return "SRational";
		if (type_ == 11)
			return "Float";
		if (type_ == 12)
			return "Double";
	};

	DirectoryEntry DirectoryEntry::create(uint16_t tag, TiffDataType type, uint32_t count, uint32_t value)
	{
		return DirectoryEntry(tag, type, count, value);
	};

	std::ostream &operator<<(std::ostream &os, const DirectoryEntry &entry)
	{
		os << "tag: " << entry.tag_ << std::endl;
		os << "type: " << entry.type_.value() << std::endl;
		os << "count: " << entry.count_ << std::endl;
		os << "value: " << entry.value_ << std::endl;
		return os;
	};

	void TiffBinary::get_default_ifd(
			uint32_t image_width /* 0x100 */,
			uint32_t image_length /* 0x101 */,
			uint16_t compressiion /* 0x103 {1,2,32773}*/,
			uint16_t photometric_interpretation /* 0x106 {0,1} */,
			uint32_t strip_offsets /* 0x111 */,
			uint32_t rows_per_strip /* 0x116 */,
			uint32_t stripe_byte_counts /* 0x117 */,
			uint32_t x_resolution_num /* 0x11A */,
			uint32_t x_resolution_den /* 0x11A */,
			uint32_t y_resolution_num /* 0x11B */,
			uint32_t y_resolution_den /* 0x11B */,
			int16_t resolution_unit /* 0x128 {1,2,3}*/)
	{

		BinaryIFD ifd;
		int head_size = sizeof(TiffHeader);
		int offset_start = head_size + sizeof(uint16_t) + ifd.number_entry * sizeof(DirectoryEntry);
		ifd_buffer = new char[sizeof(BinaryIFD)];

		// Fill Number of Directory Entries
		// ifd.number_entry = 10;

		ifd.entry[0] = DirectoryEntry::create(0x100, TiffDataType::Long(), 1, image_width);
		ifd.entry[1] = DirectoryEntry::create(0x101, TiffDataType::Long(), 1, image_length);
		ifd.entry[2] = DirectoryEntry::create(0x103, TiffDataType::Short(), 1, compressiion);
		ifd.entry[3] = DirectoryEntry::create(0x106, TiffDataType::Short(), 1, photometric_interpretation);
		ifd.entry[4] = DirectoryEntry::create(0x111, TiffDataType::Long(), 1, strip_offsets);
		ifd.entry[5] = DirectoryEntry::create(0x116, TiffDataType::Long(), 1, rows_per_strip);
		ifd.entry[6] = DirectoryEntry::create(0x117, TiffDataType::Long(), 1, stripe_byte_counts);
		// calculate offset for X resolution

		ifd.longs[0] = x_resolution_num;
		ifd.longs[1] = x_resolution_den;
		ifd.longs[2] = y_resolution_num;
		ifd.longs[3] = y_resolution_den;

		ifd.entry[7] = DirectoryEntry::create(0x11A, TiffDataType::Rational(), 1, offset_start);

		// calculate offset for Y resolution
		ifd.entry[8] = DirectoryEntry::create(0x11B, TiffDataType::Rational(), 1, offset_start + 8);
		ifd.entry[9] = DirectoryEntry::create(0x128, TiffDataType::Short(), 1, resolution_unit);

		ifd_buffer = &ifd;
	};

	void TiffGrayscale::get_default_ifd(
			uint32_t image_width /* 0x100 */,
			uint32_t image_length /* 0x101 */,
			uint16_t bit_per_sample /* 0x102, {4,8} */,
			uint16_t compressiion /* 0x103 {1,32773}*/,
			uint16_t photometric_interpretation /* 0x106 {0,1} */,
			uint32_t strip_offsets /* 0x111 */,
			uint32_t rows_per_strip /* 0x116 */,
			uint32_t stripe_byte_counts /* 0x117 */,
			uint32_t x_resolution_num /* 0x11A */,
			uint32_t x_resolution_den /* 0x11A */,
			uint32_t y_resolution_num /* 0x11B */,
			uint32_t y_resolution_den /* 0x11B */,
			int16_t resolution_unit /* 0x128 {1,2,3}*/
	)
	{
		GrayscaleIFD ifd;
		int head_size = sizeof(TiffHeader);
		int offset_start = head_size + sizeof(uint16_t) + ifd.number_entry * sizeof(DirectoryEntry);
		ifd_buffer = new char[sizeof(GrayscaleIFD)];

		// Fill Number of Directory Entries
		// ifd.number_entry = 10;

		ifd.entry[0] = DirectoryEntry::create(0x100, TiffDataType::Long(), 1, image_width);
		ifd.entry[1] = DirectoryEntry::create(0x101, TiffDataType::Long(), 1, image_length);
		ifd.entry[1] = DirectoryEntry::create(0x102, TiffDataType::Short(), 1, bit_per_sample);
		ifd.entry[2] = DirectoryEntry::create(0x103, TiffDataType::Short(), 1, compressiion);
		ifd.entry[3] = DirectoryEntry::create(0x106, TiffDataType::Short(), 1, photometric_interpretation);
		ifd.entry[4] = DirectoryEntry::create(0x111, TiffDataType::Long(), 1, strip_offsets);
		ifd.entry[5] = DirectoryEntry::create(0x116, TiffDataType::Long(), 1, rows_per_strip);
		ifd.entry[6] = DirectoryEntry::create(0x117, TiffDataType::Long(), 1, stripe_byte_counts);
		// calculate offset for X resolution

		ifd.longs[0] = x_resolution_num;
		ifd.longs[1] = x_resolution_den;
		ifd.longs[2] = y_resolution_num;
		ifd.longs[3] = y_resolution_den;

		ifd.entry[7] = DirectoryEntry::create(0x11A, TiffDataType::Rational(), 1, offset_start);

		// calculate offset for Y resolution
		ifd.entry[8] = DirectoryEntry::create(0x11B, TiffDataType::Rational(), 1, offset_start + 8);
		ifd.entry[9] = DirectoryEntry::create(0x128, TiffDataType::Short(), 1, resolution_unit);

		ifd_buffer = &ifd;
	};

	void TiffRgb::get_default_ifd(
			uint32_t image_width /* 0x100 */,
			uint32_t image_length /* 0x101 */,
			uint16_t bit_per_sample_0 /* 0x102 {(8,8,8)}*/,
			uint16_t bit_per_sample_1 /* 0x102 {(8,8,8)}*/,
			uint16_t bit_per_sample_2 /* 0x102 {(8,8,8)}*/,
			uint16_t compressiion /* 0x103 {1,32773} */,
			uint16_t photometric_interpretation /* 0x106 {2} */,
			uint32_t strip_offsets /* 0x111 */,
			uint16_t samples_per_pixel /* 0x115 */,
			uint32_t rows_per_strip /* 0x116 */,
			uint32_t stripe_byte_counts /* 0x117 */,
			uint32_t x_resolution_num /* 0x11A */,
			uint32_t x_resolution_den /* 0x11A */,
			uint32_t y_resolution_num /* 0x11B */,
			uint32_t y_resolution_den /* 0x11B */,
			int16_t resolution_unit /* 0x128 */
	)
	{

		int head_size = sizeof(TiffHeader);
		RgbIFD ifd;
		int offset_start = head_size + sizeof(uint16_t) + ifd.number_entry * sizeof(DirectoryEntry);
		ifd_buffer = new char[sizeof(RgbIFD)];

		// Fill Number of Directory Entries
		// ifd.number_entry = 10;

		ifd.entry[0] = DirectoryEntry::create(0x100, TiffDataType::Long(), 1, image_width);
		ifd.entry[1] = DirectoryEntry::create(0x101, TiffDataType::Long(), 1, image_length);
		ifd.entry[1] = DirectoryEntry::create(0x102, TiffDataType::Short(), 3, offset_start);
		ifd.shorts[0] = bit_per_sample_0;
		ifd.shorts[1] = bit_per_sample_1;
		ifd.shorts[2] = bit_per_sample_2;

		ifd.entry[2] = DirectoryEntry::create(0x103, TiffDataType::Short(), 1, compressiion);
		ifd.entry[3] = DirectoryEntry::create(0x106, TiffDataType::Short(), 1, photometric_interpretation);
		ifd.entry[4] = DirectoryEntry::create(0x111, TiffDataType::Long(), 1, strip_offsets);
		ifd.entry[5] = DirectoryEntry::create(0x116, TiffDataType::Long(), 1, rows_per_strip);
		ifd.entry[6] = DirectoryEntry::create(0x117, TiffDataType::Long(), 1, stripe_byte_counts);
		// calculate offset for X resolution

		ifd.longs[0] = x_resolution_num;
		ifd.longs[1] = x_resolution_den;
		ifd.longs[2] = y_resolution_num;
		ifd.longs[3] = y_resolution_den;

		ifd.entry[7] = DirectoryEntry::create(0x11A, TiffDataType::Rational(), 1, offset_start + 6);

		// calculate offset for Y resolution
		ifd.entry[8] = DirectoryEntry::create(0x11B, TiffDataType::Rational(), 1, offset_start + 14);
		ifd.entry[9] = DirectoryEntry::create(0x128, TiffDataType::Short(), 1, resolution_unit);

		ifd_buffer = &ifd;
	};

	void Tiff::dump(std::string filename)
	{
		std::ofstream ofs(filename.c_str(), std::ios::trunc | std::ios::binary);
		if (!ofs)
		{
			return;
		}
		char *tmp;
		TiffHeader head = TiffHeader::get_header_little_endien();
		tmp = (char *)&(head);
		ofs.write(tmp, sizeof(TiffHeader));
		tmp = (char *)ifd_buffer;
		ofs.write(tmp, sizeof(IFD));
		uint32_t zero = 0;
		tmp = (char *)&zero;
		ofs.write(tmp, sizeof(uint32_t));

		// Write data

		// close
		ofs.flush();
		ofs.close();
	};

};