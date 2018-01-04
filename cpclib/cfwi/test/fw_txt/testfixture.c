#include "stdint.h"
#include "cfwi/cfwi.h"

#define hexchar(i) ( ( (i) < 10 ) ? ( '0' + (i) ) : ( 'A' - 10 + (i) ) )

enum
{
        x_margin = ( 40 - 33 ) / 2 + 2,
        y_margin = ( 25 - 16 ) / 2 + 2
};

static const char const message[] = "\226\220\232\235 Amstrad CPC character set \227\232\220\234";

uint8_t perform_test( void )
{
        fw_scr_initialise();
        fw_scr_set_border( 2, 2 );

        fw_txt_set_pen( 3 );
        {
                uint8_t column = 20 - ( sizeof( message ) - 1 ) / 2;
                //( 40 - ( sizeof( message )/sizeof(char) - 1 ) ) >> 1;
                fw_txt_set_cursor( 2, column );
                cfwi_txt_str0_output( message );
        }

        fw_txt_set_pen( 2 );

        {
                uint8_t i = 0;

                do
                {
                        uint8_t x = ( ( i & 0x0f ) << 1 ) + x_margin;
                        unsigned char c = hexchar( i );
                        fw_txt_set_cursor( y_margin - 2, x );
                        fw_txt_wr_char( c );
                        fw_txt_set_cursor( y_margin + 17, x );
                        fw_txt_wr_char( c );
                }
                while ( ++i != 16 );
        }

        {
                uint8_t i = 0;
                const uint8_t x1 = x_margin - 2;
                const uint8_t x2 = x_margin + 32;

                do
                {
                        uint8_t y = y_margin + i;
                        unsigned char c = hexchar( i );
                        fw_txt_set_cursor( y, x1 );
                        fw_txt_wr_char( c );
                        fw_txt_set_cursor( y, x2 );
                        fw_txt_wr_char( c );
                }
                while ( ++i != 16 );
        }

        fw_txt_set_pen( 1 );

        {
                uint8_t i = 0;

                do
                {
                        uint8_t x = ( ( i & 0x0f ) << 1 ) + x_margin;
                        uint8_t y = ( i >> 4 ) + y_margin;
                        fw_txt_set_cursor( y, x );
                        fw_txt_wr_char( i );
                }
                while ( ++i != 0 );
        }
        return 0;
}
