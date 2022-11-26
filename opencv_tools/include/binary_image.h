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

#include <opencv2/opencv.hpp>

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
        int8_t byte;

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

    // int DecodeJpeg(Bitmap *bitmap, std::string filename);
    // int EncodeJpeg(Bitmap *bitmap, std::string filename);

    class BinaryImage
    {
        BytePixels *data;
    };
};

#endif