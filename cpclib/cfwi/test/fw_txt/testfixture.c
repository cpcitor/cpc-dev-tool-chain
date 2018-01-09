#include "stdint.h"
#include "cfwi/cfwi.h"

enum
{
        x_margin = ( 39 - 32 ),
        y_margin = ( 24 - 16 )
};

uint8_t perform_test( void )
{
        fw_scr_initialise();
        fw_scr_set_border( 2, 2 );

	fw_txt_set_pen(2);
	
	{
                uint8_t i = 0;

                fw_txt_set_row( y_margin - 2 );
                do
                {
                        uint8_t x = ( ( i & 0x0f ) << 1 ) + x_margin;
                        fw_txt_set_column( x );
                        fw_txt_wr_char( ( i < 10 ) ? ( '0' + i ) : ( 'A' - 10 + i ) );
                }
                while ( ++i != 16 );
        }

        {
                uint8_t i = 0;


                do
                {
                        uint8_t y = y_margin + i;
                        uint8_t x = x_margin - 2;
                        fw_txt_set_cursor( y, x );
                        fw_txt_wr_char( ( i < 10 ) ? ( '0' + i ) : ( 'A' - 10 + i ) );
                }
                while ( ++i != 16 );
        }

	fw_txt_set_pen(1);

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
