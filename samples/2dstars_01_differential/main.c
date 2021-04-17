#include "cfwi/cfwi.h"
#include "stdint.h"
#include <stdio.h>
#include <stdlib.h> //random

uint8_t __at( 0x1000 ) star_4pixelsperframe_pos_byte_l[256];
uint8_t __at( 0x1100 ) star_4pixelsperframe_pos_byte_h[256];
uint8_t __at( 0x1200 ) star_1pixel_perframe_pos_byte_l[256];
uint8_t __at( 0x1300 ) star_1pixel_perframe_pos_byte_h[256];
uint8_t __at( 0x1400 ) star_1pixel_perframe_mask[256];

void show_stars( void );

const ink_vector4 palette = { {
    /* border */
    hardware_color_r0_g0_b0_black,
    /* 16 palette entries */
    hardware_color_r0_g0_b0_black,
    hardware_color_r1_g1_b0_yellow,
    hardware_color_r0_g0_b2_bright_blue,
    hardware_color_r2_g2_b2_bright_white,
} };

void
main( void )
{
    fw_scr_set_mode( 1 );

    fw_kl_choke_off__ignore_return_value();
    fw_mc_set_inks__4( &palette );

    cfwi_txt_str0_output( "Staaars!  Reset CPC or exit emulator to stop." );

    for ( int i4 = 0; i4 < 256; i4++ )
    {
        uint16_t addr = rand();

        star_4pixelsperframe_pos_byte_l[i4] = addr & 0xff;
        star_4pixelsperframe_pos_byte_h[i4] = 0xc0 | ( ( addr >> 8 ) & 0x3f );
    }

    uint8_t masks[] =
    {
        0x10,
        0x20,
        0x40,
        0x80
    };

    for ( int i1 = 0; i1 < 256; i1++ )
    {
        uint16_t pos = rand();

        star_1pixel_perframe_mask[i1] = masks[pos&0x3];
        star_1pixel_perframe_pos_byte_l[i1] = (pos>>2) & 0xff;
        star_1pixel_perframe_pos_byte_h[i1] = 0xc0 | ( ( pos >> 10 ) & 0x7f );
    }

    show_stars();
}
