#include "stdint.h"

#define FW_V11_AND_ABOVE

#include "cfwi/cfwi.h"

#define NL "\015\012"

#define hexchar(i) ( ( (i) < 10 ) ? ( '0' + (i) ) : ( 'A' - 10 + (i) ) )

void
print_uint8_as_hex( uint8_t v )
__z88dk_fastcall
{
        fw_txt_wr_char( hexchar( v >> 4 ) );
        fw_txt_wr_char( hexchar( v & 0x0F ) );
}

void
print_uint16_as_hex( uint16_t v )
__z88dk_fastcall
{
        print_uint8_as_hex( v >> 8 );
        print_uint8_as_hex( v );
}

uint8_t perform_test( void )
{
        int8_t shift_lock;

        for ( shift_lock = 0; shift_lock != -1 ; shift_lock-- )
        {
                int8_t caps_lock;

                for ( caps_lock = 0; caps_lock != -1 ; caps_lock-- )
                {
                        uint16_t locks = caps_lock << 8 | shift_lock;
                        print_uint16_as_hex( locks );
                        fw_km_set_locks( locks );

                        {
                                uint16_t locks_check = fw_km_get_state();
                                print_uint16_as_hex( locks_check );

                                uint8_t pass = ( code == expectedcode );
                                cfwi_txt_str0_output( ( pass ) ? "PASS" : "FAIL" );
                                fw_mc_send_printer( ( pass ) ? '+' : '!' );
                        }

                        return 0;
                }

        }
}
