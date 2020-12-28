#include "fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_256__xmy_lt_256.h"
#include <cfwi/cfwi.h>
#include <stdint.h>

void
fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_256__xmy_lt_256_test()
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
            uint8_t z = fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_256__xmy_lt_256(
                x << 8 ^ y );

            uint8_t color = z / 4;

            fw_gra_set_pen( color );

            fw_gra_plot_absolute( ( x + 64 ) << 1, y << 1 );
        } while ( y != 0 );
    } while ( x != 0 );
}
