#include "cfwi/cfwi.h"
#include "printer.h"
#include "stdint.h"
#include <stdio.h>

uint16_t mult_u8_u8_squares_table[256];

void
fill_table( void )
{
    uint16_t *p = &( mult_u8_u8_squares_table[0] );
    uint16_t s = 0;
    uint8_t i = 0;

    do
    {
        // s = i * i ;
        *( p++ ) = s >> 2;
        // (x+1)² = x² + 2*x + 1
        s += ( i << 1 ) + 1;
    } while ( ++i != 0 );
    fw_mc_send_printer( '\n' );
}

uint8_t
perform_test( void )
{
    fw_scr_set_ink( 1, 18, 18 );
    fw_scr_set_ink( 2, 15, 15 );
    fw_scr_set_ink( 3, 6, 6 );
    fw_txt_win_enable( 32, 39, 0, 25 );
    printf( "Filling table..." );
    uint32_t time_0 = fw_kl_time_please();
    fill_table();
    uint32_t time_1 = fw_kl_time_please();
    printf( "done.\n" );
    printer_uint32_as_hex_with_prefix( time_1 - time_0 );
    fw_mc_send_printer( '\n' );

    {
        uint8_t y = 0;
        do
        {
            printer_uint8_as_hex_with_prefix( mult_u8_u8_squares_table[y] );
            y++;
        } while ( y != 0 );
        fw_mc_send_printer( '\n' );
    }

    uint8_t y = 0;

    do
    {
        printf( "%d ", y );
        printer_uint8_as_hex_with_prefix( y );
        fw_mc_send_printer( ' ' );
        uint8_t x = y;

        do
        {
            uint16_t tz = x * y;
            printer_uint16_as_hex_with_prefix( tz );
            fw_mc_send_printer( '=' );

            // 4 * x * y = ( x + y )² - ( x - y )²
            // x * y = ( x + y )² / 4 - ( x - y )² / 4
            // x * y = table( x + y ) - table( x - y )
            uint16_t z = mult_u8_u8_squares_table[x + y] -
                         mult_u8_u8_squares_table[x - y];
            printer_uint16_as_hex_with_prefix( z );

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
            x++;
        } while ( ( x != 0 ) && ( x < ( 256 - y ) ) );

        fw_mc_send_printer( '\n' );
        y++;
    } while ( y < 128 );

    return 0;
}
