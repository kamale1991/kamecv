#include "../include/morphology.h"

using namespace PointShape;

#include "../include/morphology.h"
#include <limits>

void StructureElement::erosion(GrayScaleImage *operand)
{
	MapParam mp_img = operand->get_map_parameter();

	uint8_t bn = 0;
	uint8_t fx = 0;
	uint8_t fx_bn = 0;
	uint8_t out_pix = 0;
	for (uint64_t ix1 = 0; ix1 < mp_img.Nx1; ix1++)
	{
		for (uint64_t ix2 = 0; ix2 < mp_img.Nx2; ix2++)
		{
			out_pix = std::numeric_limits<uint8_t>::max();
			for (uint64_t jx1 = 0; jx1 < Nx1_; jx1++)
			{
				if (ix1 < center_offset_x1_ - jx1)
				{
					continue;
				}

				for (int64_t jx2 = 0; jx2 < Nx2_; jx2++)
				{
					if (ix2 < center_offset_x2_ - jx2)
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

					if (fx_bn < out_pix)
					{
						out_pix = fx_bn;
					}
				}
			}
			operand->set(ix1, ix2, out_pix);
		}
	}
};