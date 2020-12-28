#include <cdtc/cdtc_screen_basic.h>
//#include "print.h"
#include <cdtc/uint16_t_as_uint8_t_pair.h>
#include <cfwi/cfwi.h>
#include <stdint.h>

uint16_t_as_uint8_t_pair_t coords_screen_basic__plot_test;

void
cdtc_screen_basic__plot_test( void )
{
    uint32_t time_0 = fw_kl_time_please();
    {
        do
        {

        } while ( ( ++coords_screen_basic__plot_test.as_uint16_t ) != 0 );
    }
    uint32_t time_1 = fw_kl_time_please();
    {
        do
        {
            fw_screen_basic__plot_absolute( coords_screen_basic__plot_test.as_uint8_pair.low,
                                  coords_screen_basic__plot_test.as_uint8_pair.high );
        } while ( ( ++coords_screen_basic__plot_test.as_uint16_t ) != 0 );
    }
    uint32_t time_2 = fw_kl_time_please();
    {
        do
        {
            cdtc_screen_basic__plot( coords_screen_basic__plot_test.as_uint16_t );
        } while ( ( ++coords_screen_basic__plot_test.as_uint16_t ) != 0 );
    }
    uint32_t time_3 = fw_kl_time_please();

    uint32_t neutral = time_1 - time_0;
    uint32_t fw = time_2 - time_1 - neutral;
    uint32_t cdtc = time_3 - time_2 - neutral;

    /* print_uint32_as_hex( neutral ); */
    /* print_lf(); */
    /* print_uint32_as_hex( fw ); */
    /* print_lf(); */
    /* print_uint32_as_hex( cdtc ); */
    /* print_lf(); */

    // Results : neutral 0194
    // Fw 2DFE
    // Cdtc 04B3
    // cdtc/fw = 0,1021
    // 0x04B3 = 1203 interrupts of 1/300s or 4,01 seconds to draw 65536 pixels.
    // Or 16343 pixels / second.
    // Or 326 pixels / frame.
    return;
}
