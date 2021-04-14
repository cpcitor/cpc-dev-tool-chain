#include "cfwi/cfwi.h"

void
main()
{
    {
        static unsigned char x, y;

        for ( x = 0; x < 255; x++ )
        {

            for ( y = 0; y < 255; y++ )
            {
                if ( ( x ^ y ) > x )
                {
                    fw_gra_plot_absolute( 2 * x + 1, 2 * y + 1 );
                }
            }
        }
    }

    cfwi_txt_str0_output( "Press any key to exit." );
    fw_km_wait_key();
}
