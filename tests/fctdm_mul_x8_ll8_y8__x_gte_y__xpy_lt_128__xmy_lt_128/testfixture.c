#include "cdtc/fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128.h"
#include "cfwi/cfwi.h"
#include "printer.h"
#include "stdint.h"
#include <stdio.h>

uint16_t mult_u8_u8_squares_table[256];

uint8_t
perform_test( void )
{
    fw_scr_set_ink( 1, 18, 18 );
    fw_scr_set_ink( 2, 15, 15 );
    fw_scr_set_ink( 3, 6, 6 );
    fw_txt_win_enable( 32, 39, 0, 25 );
    printf( "Filling table..." );
    uint32_t time_0 = fw_kl_time_please();
    fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128_fill_table();
    uint32_t time_1 = fw_kl_time_please();
    printf( "done.\n" );

    {
        uint8_t y=0;
        do
        {
            printer_uint8_as_hex_with_prefix( fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128_table[y] );
            y++;
        } while (y!=0);
        fw_mc_send_printer( '\n' );
    }

    uint8_t y = 0;

    do
    {
        --y;
        printf( "%d ", y );
        printer_uint8_as_hex_with_prefix( y );
        fw_mc_send_printer( ' ' );
        uint8_t x = 0;

        do
        {
            --x;
            // 4 * x * y = ( x + y )² - ( x - y )²
            // x * y = ( x + y )² / 4 - ( x - y )² / 4
            // x * y = table( x + y ) - table( x - y )
            uint16_t z = fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128( (x << 8) | y);
            printer_uint16_as_hex_with_prefix( z );

            uint16_t tz = x * y;

            uint8_t color = 0;

            switch ( tz - z )
            {
            case 0:
                color = 1;
                break;

            case -1:
            case 1:
                color = 2;
                break;

            default:
                color = 3;
            };

            fw_gra_set_pen( color );

            fw_gra_plot_absolute( x << 1, y << 1 );
        } while ( x != 0 );

        fw_mc_send_printer( '\n' );
    } while ( y != 0 );

    return 0;
}
