#include "../include/morphology.h"

using namespace PointShape;

#include "../include/morphology.h"
#include <limits>

void StructureElement::erosion(GrayScaleImage *operand, GrayScaleImage *out)
{
	MapParam mp_img = operand->get_map_parameter();

	uint8_t bn = 0;
	uint8_t fx = 0;
	uint8_t fx_bn = 0;
	uint8_t out_pix = 0;
	uint64_t jx1_start, jx1_end;
	uint64_t jx2_start, jx2_end;
	uint64_t ix1_residual_pixel, ix2_residual_pixel;

	// uint64_t se_

	for (uint64_t ix1 = 0; ix1 < mp_img.Nx1; ix1++)
	{
		uint64_t ix1_unprocessed = mp_img.Nx1 - 1 - ix1;
		for (uint64_t ix2 = 0; ix2 < mp_img.Nx2; ix2++)
		{
			uint64_t ix2_unprocessed = mp_img.Nx2 - 1 - ix2;
			out_pix = 255;
			jx1_start = 0;
			jx1_end = Nx1_;
			if (ix1 < center_offset_x1_)
			{
				jx1_start = center_offset_x1_;
			}
			if (ix1 + Nx1_ - 1 - center_offset_x1_ > mp_img.Nx1 - 1)
			{
				// jx1_end = center_offset_x1_ + ;
			}

			for (uint64_t jx1 = jx1_start; jx1 < jx1_end; jx1++)
			{
				if (ix1 < center_offset_x1_ - jx1 + 1)
				{
					continue;
				}

				for (int64_t jx2 = 0; jx2 < Nx2_; jx2++)
				{
					if (ix2 + jx2 < center_offset_x2_)
					{
						continue;
					}

					bn = get(jx1, jx2);
					fx = operand->get(ix1 + jx1 - center_offset_x1_, ix2 + jx2 - center_offset_x2_);
					if (fx > bn)
					{
						fx_bn = fx - bn;
					}
					else
					{
						fx_bn = 0;
					}
					std::cout << "ix1=" << ix1 << " ix2=" << ix2 << " jx1=" << jx1 << " jx2=" << jx2 << " fx_bn=" << (int)fx_bn << " fx=" << (int)fx << " bn=" << (int)bn << std::endl;

					if (fx_bn < out_pix)
					{
						out_pix = fx_bn;
					}
				}
			}
			out->set(ix1, ix2, out_pix);
		}
	}
};
