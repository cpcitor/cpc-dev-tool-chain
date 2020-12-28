#include "fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128.h"
#include <stdint.h>
// #include "print.h"
#include <cdtc/uint16_t_as_uint8_t_pair.h>
#include <cfwi/cfwi.h>

void
fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128_test( void )
{
    uint8_t x = 0;

    do
    {
        --x;
        // print_uint8_as_hex( x );
        uint8_t y = 200;

        /* if ( y > x ) */
        /* { */
        /*         y = x + 1; */
        /* } */

        do
        {
            --y;

            uint16_t_as_uint8_t_pair_t bytepair = {
                .as_uint8_pair = { .low = y, .high = x } };

            uint8_t z = fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128(
                bytepair.as_uint16_t );

            uint8_t color = z / 4;

            fw_gra_set_pen( color );

            fw_gra_plot_absolute( x << 1, y << 1 );
        } while ( y != 0 );
    } while ( x != 0 );
}
