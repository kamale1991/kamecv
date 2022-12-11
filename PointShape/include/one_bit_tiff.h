/**
 * @file one_bit_tiff.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-06-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <bitset>
#include <string>
#include <fstream>

namespace BI
{

	// Header
	// IFD
	//

	class TiffHeader
	{
		const char byte_order_0_;
		const char byte_order_1_;
		const uint16_t magic_number_ = 42;
		const uint32_t offset_ = 8; // Offset of 0th IFD
		TiffHeader(char byte_order_0, char byte_order_1) : byte_order_0_(byte_order_0), byte_order_1_(byte_order_1){};

	public:
		static TiffHeader get_header_little_endien();
		static TiffHeader get_header_big_endien();
		friend std::ostream &operator<<(std::ostream &os, const TiffHeader &head);
	};

	class TiffDataType
	{
	private:
		int type_;

		constexpr explicit TiffDataType(const int type) : type_(type){};

	public:
		TiffDataType() = default;
		constexpr const int value() const;
		constexpr const char *name() const;

		static const TiffDataType Byte(int type = 1) { return TiffDataType(1); };
		static const TiffDataType Ascii(int type = 2) { return TiffDataType(2); };
		static const TiffDataType Short(int type = 3) { return TiffDataType(3); };
		static const TiffDataType Long(int type = 4) { return TiffDataType(4); };
		static const TiffDataType Rational(int type = 5) { return TiffDataType(5); };
		static const TiffDataType SByte(int type = 6) { return TiffDataType(6); };
		static const TiffDataType Undefined(int type = 7) { return TiffDataType(7); };
		static const TiffDataType SShort(int type = 8) { return TiffDataType(8); };
		static const TiffDataType SLong(int type = 9) { return TiffDataType(9); };
		static const TiffDataType SRational(int type = 10) { return TiffDataType(10); };
		static const TiffDataType Float(int type = 11) { return TiffDataType(11); };
		static const TiffDataType Double(int type = 12) { return TiffDataType(12); };

		// friend std::ostream &operator<<(std::ostream &os, const TiffDataType &type);
	};

	class DirectoryEntry
	{
		uint16_t tag_;
		TiffDataType type_;
		uint32_t count_;
		uint32_t value_; // or offset

	public:
		DirectoryEntry(){};
		DirectoryEntry(uint16_t tag, TiffDataType type, uint32_t count, uint32_t value) : tag_(tag), type_(type), count_(count), value_(value){};
		static DirectoryEntry create(uint16_t tag, TiffDataType type, uint32_t count, uint32_t value);

		friend std::ostream &operator<<(std::ostream &os, const DirectoryEntry &head);
	};

	struct IFD
	{
	};

	struct BinaryIFD : public IFD
	{
		const uint16_t number_entry = 10;
		DirectoryEntry entry[10];
		uint32_t longs[4];
		BinaryIFD(){};
	};

	struct GrayscaleIFD : public IFD
	{
		const uint16_t number_entry = 11;
		DirectoryEntry entry[11];
		uint32_t longs[4];
		GrayscaleIFD(){};
	};

	struct RgbIFD : public IFD
	{
		const uint16_t number_entry = 12;
		DirectoryEntry entry[12];
		uint16_t shorts[3];
		uint32_t longs[4];
		RgbIFD(){};
	};

	class Tiff
	{
	protected:
		void *ifd_buffer;
		IFD ifd_;

	public:
		Tiff(IFD ifd) : ifd_(ifd){};
		void dump(std::string filename);
		void get_default_ifd(){};
	};

	class TiffBinary : public Tiff
	{
	public:
		TiffBinary(IFD ifd) : Tiff(ifd){};
		void get_default_ifd(
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
				int16_t resolution_unit /* 0x128 {1,2,3}*/
		);
	};

	class TiffGrayscale : Tiff
	{
	public:
		TiffGrayscale(IFD ifd) : Tiff(ifd){};
		void get_default_ifd(
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
		);
	};

	class TiffRgb : Tiff
	{
	public:
		TiffRgb(IFD ifd) : Tiff(ifd){};
		void get_default_ifd(
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
		);
	};
};