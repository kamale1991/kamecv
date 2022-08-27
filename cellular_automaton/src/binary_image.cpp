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

#include "../include/binary_image.h"

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

    void Bitmap::import_image(std::string filename)
    {
        cv::Mat original_image;
        original_image = cv::imread(filename);
        std::cout << "original_image=" << original_image << std::endl;
    };

    /*
    int DecodeJpeg(Bitmap *bitmap, std::string filename)
    {
        jpeg_decompress_struct *jpeg;
        jpeg_error_mgr *err;
        FILE *file;
        jpeg->err = jpeg_std_error(err);

        file = fopen(filename.c_str(), "rb");
        if (file == NULL)
        {
            std::cout << "cannot open " << filename << std::endl;
            return -1;
        }

        jpeg_create_decompress(jpeg);
        jpeg_stdio_src(jpeg, file);
        jpeg_read_header(jpeg, TRUE);
        jpeg_start_decompress(jpeg);
        std::cout << "(width,height,ch) = (" << jpeg->output_width << "," << jpeg->output_height << "," << jpeg->output_components << std::endl;

        bitmap->data = new unsigned char[sizeof(unsigned char) * jpeg->output_width * jpeg->output_height * jpeg->output_components];

        if (bitmap->data == NULL)
        {
            std::cout << "data allocation failed" << std::endl;
            fclose(file);
            jpeg_destroy_decompress(jpeg);
            return -1;
        }

        JSAMPROW *buffer;
        for (int iy = 0; iy < jpeg->output_height; iy++)
        {
            buffer = (JSAMPROW *)(bitmap->data + iy * jpeg->output_components * jpeg->output_width);
            jpeg_read_scanlines(jpeg, buffer, 1);
        }
        bitmap->height = jpeg->output_height;
        bitmap->width = jpeg->output_width;
        bitmap->ch = jpeg->output_components;

        jpeg_finish_decompress(jpeg);
        jpeg_destroy_decompress(jpeg);
        fclose(file);
        return 0;
    };

    int EncodeJpeg(Bitmap *bitmap, std::string filename)
    {
        jpeg_compress_struct *jpeg;
        jpeg_error_mgr *err;
        FILE *file;
        jpeg->err = jpeg_std_error(err);
        jpeg_create_compress(jpeg);

        file = fopen(filename.c_str(), "wb");
        if (file == NULL)
        {
            std::cout << "cannot open " << filename << std::endl;
            jpeg_destroy_compress(jpeg);
            return -1;
        }
        jpeg_stdio_dest(jpeg, file);

        jpeg->image_width = bitmap->width;
        jpeg->image_height = bitmap->height;
        jpeg->input_components = bitmap->ch;
        jpeg->in_color_space = JCS_RGB;
        jpeg_set_defaults(jpeg);
        jpeg_set_quality(jpeg, 50, true);
        jpeg_start_compress(jpeg, true);

        JSAMPROW *buffer;
        for (int iy = 0; iy < jpeg->image_height; iy++)
        {
            buffer = (JSAMPROW *)(bitmap->data + (iy * bitmap->width * bitmap->ch));
            jpeg_write_scanlines(jpeg, buffer, 1);
        }
        jpeg_finish_compress(jpeg);
        return 0;
    };
    */
};