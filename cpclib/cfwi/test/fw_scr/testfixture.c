#include "stdint.h"
#include "cfwi/cfwi.h"
#include "random_with_8bit_lfsr.h"

uint16_t random_statex = 0xcafe;
uint16_t random_statey = 0xface;
const char stars[]= { '.', '+', 'o', 'O', 0xCA };
static const ink_vector4 palette =
{
        {
		hardware_color_r0_g0_b0_black,
		hardware_color_r0_g0_b0_black,
		hardware_color_r2_g2_b2_bright_white,
		hardware_color_r2_g1_b1_pink,
		hardware_color_r1_g1_b1_white
	}
};

uint8_t perform_test()
{
	fw_scr_initialise();
	fw_kl_choke_off__ignore_return_value(); // no ink flash please
        fw_mc_set_inks__4( &palette );

	cfwi_txt_str0_output("Test of C-level wrapper to firmware SCR functions ---- in guise of a silly demo featuring pop tart cat!");
	
	/*{
		uint8_t star_count = 40;
		while (star_count>0)
		{
			uint8_t x = random_with_16bit_lfsr(&random_statex) & 0x3f;
			uint8_t y;
			//fw_mc_send_printer(y);
			if (x>39) continue;
			y = random_with_16bit_lfsr(&random_statey) & 0x1f;
			if (y>24) continue;

			fw_txt_set_cursor(y, x);
			fw_txt_wr_char(stars[y>>6]);
			--star_count;
		}
	}*/
	//fw_km_wait_key();
	{
		uint16_t offset=((uint16_t)-2048);
		while (offset != 0)
		{
			uint8_t n = random_with_16bit_lfsr(&random_statex);
			fw_scr_set_offset(offset);
			fw_txt_set_cursor__fastcall(0x230c);
			fw_txt_set_pen( 1 );
			fw_txt_wr_char( (n<32)?0xce:0x7f ); // for rainbow
			fw_txt_set_pen( 3 );
			fw_txt_wr_char( 0xc1 ); // sort-of \ for the tail
			fw_txt_set_pen( 2 );
			fw_txt_wr_char( '[' );
			fw_txt_wr_char( ']' );
			fw_txt_set_pen( 3 );
			fw_txt_wr_char( 0xe0 ); // face
			fw_txt_set_column( 1 );
			fw_txt_set_pen( 1 );
			/*{
				uint8_t n = random_with_16bit_lfsr(&random_statex);
				if (n > 249)
				{
					fw_txt_set_cursor__fastcall(0x2718);
					fw_txt_wr_char(stars[n&3]);
				}
			}*/
			fw_txt_wr_char ( (n<5)?(stars[n]):' ' );
			//fw_mc_wait_flyback();
			offset += 2;
		}
	}
	return 0;
}
