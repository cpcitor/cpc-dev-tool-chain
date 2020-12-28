#include "fctdd_256_times_256_divided_by_lp_257_plus_y8_rp.h"
#include "print.h"
#include <cfwi/cfwi.h>
#include <stdint.h>

void
fctdd_256_times_256_divided_by_lp_257_plus_y8_rp_test( void )
{
    uint8_t x = 0;

    do
    {
        {
            uint8_t y =
                fctdd_256_times_256_divided_by_lp_257_plus_y8_rp_table[x];
            print_uint8_as_hex(
                fctdd_256_times_256_divided_by_lp_257_plus_y8_rp_table[x] );
            fw_gra_plot_absolute( x << 1, ( y - 56 ) << 1 );
        }
        ++x;
    } while ( x != 0 );
}
