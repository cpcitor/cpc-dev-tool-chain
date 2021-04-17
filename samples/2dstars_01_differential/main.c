#include "cfwi/cfwi.h"
#include "stdint.h"
#include <stdio.h>
#include <stdlib.h> //random

uint8_t __at( 0x3e00 ) star_pos_byte_l[256];
uint8_t __at( 0x3f00 ) star_pos_byte_h[256];

void show_stars( void );

const ink_vector4 palette = { {
    /* border */
    hardware_color_r0_g0_b0_black,
    /* 16 palette entries */
    hardware_color_r0_g0_b0_black,
    hardware_color_r0_g0_b2_bright_blue,
    hardware_color_r1_g1_b1_white,
    hardware_color_r2_g2_b2_bright_white,
} };

void
main( void )
{
    fw_scr_set_mode( 1 );

    fw_kl_choke_off__ignore_return_value();
    fw_mc_set_inks__4( &palette );

    cfwi_txt_str0_output( "Staaars!  Reset CPC or exit emulator to stop." );

    for ( int i = 0; i < 256; i++ )
    {
        uint16_t addr = rand();

        star_pos_byte_l[i] = addr & 0xff;
        star_pos_byte_h[i] = 0xc0 | ( ( addr >> 8 ) & 0x3f );
    }

    show_stars();
}
