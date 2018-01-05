#include "stdint.h"
#include "cfwi/cfwi.h"

#define NL "\015\012"

#define hexchar(i) ( ( (i) < 10 ) ? ( '0' + (i) ) : ( 'A' - 10 + (i) ) )

uint8_t perform_test()
{
        cfwi_txt_str0_output( "Setting tape speed to 2000 bauds." NL );
        fw_cas_set_speed( 167, 50 );

        {
                int loop = 3;

                while ( loop-- > 0 )
                {
                        fw_txt_output( 13 );
                        fw_txt_output( 10 );

                        cfwi_txt_str0_output( "Starting motor... (esc to interrupt)" NL );
                        {
                                uint8_t oldstate = fw_cas_start_motor();
                                cfwi_txt_str0_output( "CAS START MOTOR returned:" );
                                fw_txt_wr_char( hexchar( oldstate >> 4 ) );
                                fw_txt_wr_char( hexchar( oldstate & 0x0F ) );
                        }
                        cfwi_txt_str0_output( NL "Again... (esc to interrupt)" NL );
                        {
                                uint8_t oldstate = fw_cas_start_motor();
                                cfwi_txt_str0_output( "CAS START MOTOR returned:" );
                                fw_txt_wr_char( hexchar( oldstate >> 4 ) );
                                fw_txt_wr_char( hexchar( oldstate & 0x0F ) );
                        }
                        fw_txt_output( 13 );
                        fw_txt_output( 10 );

                        cfwi_txt_str0_output( "Stopping motor... (esc to interrupt)" NL );
                        {
                                uint8_t oldstate = fw_cas_stop_motor();
                                cfwi_txt_str0_output( "CAS STOP MOTOR returned:" );
                                fw_txt_wr_char( hexchar( oldstate >> 4 ) );
                                fw_txt_wr_char( hexchar( oldstate & 0x0F ) );
                        }
                        cfwi_txt_str0_output( NL "Again... (esc to interrupt)" NL );
                        {
                                uint8_t oldstate = fw_cas_stop_motor();
                                cfwi_txt_str0_output( "CAS STOP MOTOR returned:" );
                                fw_txt_wr_char( hexchar( oldstate >> 4 ) );
                                fw_txt_wr_char( hexchar( oldstate & 0x0F ) );
                        }
                        fw_txt_output( 13 );
                        fw_txt_output( 10 );
                }
        }
        return 0;
}
