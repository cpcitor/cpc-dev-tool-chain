#include "cfwi/cfwi.h"
#include "colors.h"
#include "stdint.h"
#include <stdio.h>

uint8_t
perform_test()
{
    int color_index = 0;
    for ( color_index = 0; color_index < color_count; color_index++ )
    {
        printf( "color %d is %s\015\n", color_index,
                color_to_colorname( color_index ) );
    }
    return 0;
}
