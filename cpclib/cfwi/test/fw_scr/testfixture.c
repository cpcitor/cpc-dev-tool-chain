#include "stdint.h"
#include "cfwi/cfwi.h"
#include "random_with_8bit_lfsr.h"
#include "setborder.h"

uint16_t random_statex = 0xcafe;
uint16_t random_statey = 0xface;
const char stars[] = { '.', '+', 'o', 'O', 0xCA };
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

#define hexchar(i) ( ( (i) < 10 ) ? ( '0' + (i) ) : ( 'A' - 10 + (i) ) )

void pr_x8( uint8_t n )
{
        fw_txt_wr_char( hexchar( n >> 4 ) );
        fw_txt_wr_char( hexchar( n & 0x0f ) );
}

void pr_x16( uint16_t n )
{
        pr_x8( n >> 8 );
        pr_x8( n );
}

/* void pr_uint( unsigned int n ) */
/* { */
/*         if ( n / 10 != 0 ) */
/*         { */
/*                 pr_uint( n / 10 ); */
/*         } */

/*         fw_txt_wr_char( ( n % 10 ) + '0' ); */
/* } */

#define NL "\015\012"
#define dbgvar_u8(VARNAME) { cfwi_txt_str0_output( #VARNAME ); fw_txt_wr_char('=0x'); pr_x8(VARNAME); cfwi_txt_str0_output(NL) ; }
#define dbgvar_u16(VARNAME) { cfwi_txt_str0_output( #VARNAME ); fw_txt_wr_char('=0x'); pr_x16(VARNAME); cfwi_txt_str0_output(NL) ; }

uint8_t perform_test()
{
        {
                fw_scr_screen_location_t screen_location;
                int i;
                screen_location.as_uint32_t = fw_scr_get_location();
                dbgvar_u8( screen_location.base_address_msb );

                for ( i = 0; i < 30; i++ )
                {
                        screen_location.as_uint32_t = fw_scr_get_location();

                        dbgvar_u16( screen_location.offset );
                }
        }

        fw_km_wait_key();

        fw_scr_initialise();
        fw_kl_choke_off__ignore_return_value(); // no ink flash please
        fw_mc_set_inks__4( &palette );

        {
                const char const marquee[] = "Test of C-level wrapper to firmware SCR functions"
                                             " ---- "
                                             "in guise of a silly demo featuring pop tart cat!"
                                             " ---- "
                                             "Border color changes are not done by firmware, though.";
                char const *p = marquee;
                uint8_t pen = 1;

                while ( *p != 0 )
                {
                        fw_txt_set_pen( pen );

                        if ( ++pen == 4 )
                        {
                                pen = 1;
                        }

                        fw_txt_wr_char( *p );
                        p++;
                }
        }

        fw_txt_set_pen( 1 );

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
                uint16_t offset = ( ( uint16_t ) -2048 );

                while ( offset != 0 )
                {
                        uint8_t n = random_with_16bit_lfsr( &random_statex );
                        fw_mc_wait_flyback();
                        fw_scr_set_offset( offset );
                        fw_txt_set_cursor__fastcall( 0x230c );
                        setborder_or40( hardware_color_r2_g0_b0_bright_red );
                        fw_txt_wr_char( ( n < 32 ) ? 0xce : 0x7f ); // for rainbow
                        setborder_or40( hardware_color_r2_g1_b0_orange );
                        fw_txt_wr_char( 0xc1 ); // sort-of \ for the tail
                        setborder_or40( hardware_color_r2_g2_b0_bright_yellow );
                        fw_txt_wr_char( '[' );
                        setborder_or40( hardware_color_r0_g2_b0_bright_green );
                        fw_txt_wr_char( ']' );
                        setborder_or40( hardware_color_r0_g1_b2_sky_blue );
                        fw_txt_wr_char( 0xe0 ); // face
                        setborder_or40( hardware_color_r1_g0_b1_magenta );
                        fw_txt_set_column( 1 );
                        /*fw_txt_set_pen( 1 );*/
                        /*{
                                uint8_t n = random_with_16bit_lfsr(&random_statex);
                                if (n > 249)
                                {
                                        fw_txt_set_cursor__fastcall(0x2718);
                                        fw_txt_wr_char(stars[n&3]);
                                }
                        }*/
                        fw_txt_wr_char( ( n < 5 ) ? ( stars[n] ) : ' ' );
                        offset += 2;
                        setborder_or40( hardware_color_r0_g0_b0_black );
                }
        }
        return 0;
}
