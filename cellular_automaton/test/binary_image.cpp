/**
 * @file binary_image.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-06-03
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "gtest/gtest.h"
#include "../include/binary_image.h"

namespace BI
{
    TEST(BytePixels, set)
    {
        BytePixels ebpix;
        ebpix.set(0, 1);
        int8_t i = 0b00000001;
        BytePixels golden = BytePixels(i);
        EXPECT_EQ(true, ebpix == golden);

        ebpix.set(7, 1);
        i = 0b10000001;
        golden = BytePixels(i);
        EXPECT_EQ(true, ebpix == golden);

        ebpix.set(7, 0);
        EXPECT_EQ(true, ebpix == BytePixels(0b00000001));

        ebpix.set(0, 0);
        EXPECT_EQ(true, ebpix == BytePixels(0b00000000));

        ebpix.one();
        EXPECT_EQ(true, ebpix == BytePixels(0b11111111));

        ebpix.zero();
        EXPECT_EQ(true, ebpix == BytePixels(0b00000000));
    };

    TEST(DecodeJpeg, test)
    {
        Bitmap *bitmap = new Bitmap;
        std::string filename = "./opencv_sample/home.jpg";
        bitmap->import_image(filename);
        // DecodeJpeg(bitmap, filename);
        // bitmap->show_head();
        // bitmap->show_data();
    }
};