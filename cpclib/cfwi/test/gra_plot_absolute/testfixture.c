#include "cfwi/cfwi.h"
#include "stdint.h"

uint8_t perform_test()
{
	{
		static unsigned char x, y;

		for (x = 0; x < 255; x++)
		{

			for (y = 0; y < 255; y++)
			{
				if ((x ^ y) > x)
				{
					fw_gra_plot_absolute(2 * x + 1, 2 * y + 1);
				}
			}

		}
	}
	return 0;
}
