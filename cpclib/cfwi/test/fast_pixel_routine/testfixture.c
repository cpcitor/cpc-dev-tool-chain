#include "cfwi/cfwi.h"
#include "stdint.h"

uint8_t *address_of_screen_line[200];
uint16_t tmp0;

uint8_t perform_test()
{
#include "prepare_screen_table_asm"
    {
        static unsigned char x, y;

        for ( y = 0; y < 200; y++ )
        {
            uint8_t *lineaddr = address_of_screen_line[y];
            for ( x = 255; x != 0; x-- )
            {
                if ( ( x ^ y ) > x )
                {
                    uint8_t *addr = lineaddr + (x >> 2);
                    uint8_t mask = 0x88 >> (x & 3);
                    /* switch (x & 3) */
                    /* { */
                    /* case 0: */
                    /*     mask = 0x88; */
                    /* case 1: */
                    /*     mask = 0x44; */
                    /* case 2: */
                    /*     mask  */
                    *addr |= (mask & 0xF0);
                    // fw_gra_plot_absolute( 2 * x + 1, 2 * y + 1 );
                }
            }

        }
    }
    return 0;
}
