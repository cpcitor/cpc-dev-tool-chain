#include "printer.h"
#include "stdint.h"
#include "cfwi/cfwi.h"

#define hexchar(i) ( ( (i) < 10 ) ? ( '0' + (i) ) : ( 'A' - 10 + (i) ) )

void printer_uint16( uint16_t n )
{
        if ( n / 10 != 0 )
        {
                printer_uint16( n / 10 );
        }

        fw_mc_send_printer( ( n % 10 ) + '0' );
}

void printer_uint8_as_hex( uint8_t v ) __z88dk_fastcall
{
        fw_mc_send_printer( hexchar( v >> 4 ) );
        fw_mc_send_printer( hexchar( v & 0x0F ) );
}

void printer_uint8_as_hex_with_prefix( uint8_t v ) __z88dk_fastcall
{
        fw_mc_send_printer( ' ' );
        fw_mc_send_printer( '0' );
        fw_mc_send_printer( 'x' );
        printer_uint8_as_hex( v );
}

void printer_uint16_as_hex_with_prefix( uint16_t v ) __z88dk_fastcall
{
        fw_mc_send_printer( ' ' );
        fw_mc_send_printer( '0' );
        fw_mc_send_printer( 'x' );
        printer_uint8_as_hex( v >> 8 );
        printer_uint8_as_hex( v );
}

void printer_uint32_as_hex_with_prefix( uint32_t v ) __z88dk_fastcall
{
        fw_mc_send_printer( ' ' );
        fw_mc_send_printer( '0' );
        fw_mc_send_printer( 'x' );
        printer_uint8_as_hex( v >> 24 );
        printer_uint8_as_hex( v >> 16 );
        printer_uint8_as_hex( v >> 8 );
        printer_uint8_as_hex( v );
}
