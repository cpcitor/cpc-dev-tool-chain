#ifndef  __FW_CAS_H__
#define __FW_CAS_H__

#include <stdint.h>
void fw_cas_in_abandon(void);
void fw_cas_return(void);
void fw_cas_out_abandon(void);

/** 119: CAS INITIALISE
    #BC65
    Initialize the Cassette Manager.
    Action:
    Full initialization of the Cassette Manager (as used during EMS).
    Entry conditions:
    No conditions.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    Operations carried out are:
    All streams are marked closed.
    The default write speed is set up.
    The prompt messages are turned off.
    The cassette motor is turned off (except on V1.0 firmware).
    Related entries:
    CAS
    CAS
    CAS
    CAS
    CAS
    IN ABANDON
    NOISY
    OUT ABANDON
    SET SPEED
    STOP MOTOR
*/
void fw_cas_initialise(void) __preserves_regs(iyh, iyl);

/** 120: CAS SET SPEED
    #BC68
    Set the write speed.
    Action:
    Set the length to write bits and the amount of write precompensation to apply.
    Entry conditions:
    HL contains the length of half a zero bit.
    A contains the precompensation to apply.
    Exit conditions:
    AF and HL corrupt.
    All other registers preserved.
    Notes:
    The speed supplied is the length of half a zero bit in microseconds. A one bit is
    written as twice the length of a zero bit. The speed supplied can be related to the
    average baud rate (assuming equal numbers of ones and zeros) by the following
    equation:
    Average baud rate
    = 1 000 000 / (3 * Halfzero length)
    = 333 333 / Halfzero length
    The halfzero length must lie between 130 and 480 microseconds. Values outside this
    range will cause read and write errors.
    The precompensation supplied is the extra length, in microseconds, to add to half a
    one bit and to subtract from half a zero bit under certain conditions. The amount of
    precompensation required varies with the speed (more is required at higher baud
    rates).
    The precompensation may lie between 0 and 255 microseconds although the higher
    settings are not useful as they will cause read and write errors.
    The default half zero length and precompensation settings are 333 microseconds
    (1000 baud) and 25 microseconds respectively. The commonly used faster setting is
    167 microseconds (2000 baud) with 50 microseconds of precompensation. These
    values have been determined after extensive testing and the user is advised to stick to
    them.
    Related entries:
    CAS INITIALISE
*/
void fw_cas_set_speed(uint16_t length_of_half_zero_bit, uint8_t precompensation) __z88dk_callee __preserves_regs(d, e, iyh, iyl);

/** 121: CAS NOISY
    #BC6B
    Enable or disable prompt messages.
    Action:
    Disabling messages will prevent the prompt and information messages from being
    printed. It will not prevent error messages from being printed. Enabling messages
    allows all messages to be printed.
    Entry conditions:
    If messages are to be enabled:
    A must be zero.
    If messages are to be disabled:
    A must be non-zero.
    Exit conditions:
    AF corrupt.
    All other registers preserved.
    Notes:
    The prompt and information messages which are turned off are:
    Press PLAY then any key:
    Press REC and PLAY then any key:
    Found <FILENAME> block <N>
    Loading <FILENAME> block <N>
    Saving <FILENAME> block <N>
    The error messages which are not turned off are:
    Read error <x>
    Write error a
    Rewind tape
    Related entries:
    CAS INITIALISE
*/
void fw_cas_noisy(uint8_t messages_enabled) __z88dk_fastcall __preserves_regs(b, c, d, e, h, l, iyh, iyl);

/** #### CFWI-specific information: ####

    There are only 2 possible values returned by firmware in A, and firmware may set carry.

    A=0x00 motor was OFF
    A=0x10 motor was ON
    NOCARRY: interrupted by escape
    CARRY: run to end without interruption

    C level function returns the value of A with bit zero changed.

    Use like this:

    uint8_t rc = fw_cas_start_motor();
    uint8_t motor_state = (rc&0x10);
    if (rc&1)
    {
    // was interrupted
    }


    122: CAS START MOTOR
    #BC6E
    Start the cassette motor.
    Action:
    Turn the cassette motor on and wait for it to pick up speed if it was previously off.
    Entry conditions:
    No conditions.
    Exit conditions:
    If the motor turned on OK:
    Carry true.
    If the user hit escape:
    Carry false.
    Always:
    A contains the previous motor state.
    Other flags corrupt.
    All other registers preserved.
    Notes:
    If the motor is not already on then the routine waits for approximately two seconds to
    allow the tape to reach full speed.
    The motor is always turned on by this routine. If the user hits the escape key then the
    time spent waiting for the motor to pick up speed is truncated.
    The previous motor state may be passed to CAS RESTORE MOTOR.
    Related entries:
    CAS RESTORE MOTOR
    CAS STOP MOTOR
*/
uint16_t fw_cas_start_motor(void) __preserves_regs(b, c, d, e, iyh, iyl);

/** #### CFWI-specific information: ####

    There are only 2 possible values returned by firmware in A, and firmware may set carry.

    A=0x00 motor was OFF
    A=0x10 motor was ON
    NOCARRY: interrupted by escape
    CARRY: run to end without interruption

    C level function returns the value of A with bit zero changed.

    Use like this:

    uint8_t rc = fw_cas_stop_motor();
    uint8_t motor_state = (rc&0x10);
    if (rc&1)
    {
    // was interrupted
    }


    123: CAS STOP MOTOR
    #BC71
    Stop the cassette motor.
    Action:
    Turn the cassette motor off and return its previous state.
    Entry conditions:
    No conditions.
    Exit conditions:
    If the motor was turned off OK:
    Carry true.
    If the user hit escape:
    Carry false.
    Always:
    A contains the previous motor state.
    Other flags corrupt.
    All other registers preserved.
    Notes:
    The motor is always turned off by this routine. There is no delay to allow the motor to
    slow down.
    The previous motor state may be passed to CAS RESTORE MOTOR.
    Related entries:
    CAS RESTORE MOTOR
    CAS START MOTOR */
uint16_t fw_cas_stop_motor(void) __preserves_regs(b, c, d, e, iyh, iyl);

/** #### CFWI-specific information: ####

    C level function returns 0xff if carry was set, else 0.

    Use like this:

    uint8_t rc = fw_cas_restore_motor();
    if (rc==0)
    {
    // was interrupted
    }


    124: CAS RESTORE MOTOR
    #BC74
    Restore previous state of cassette motor.
    Action:
    Turn the cassette motor on or off again. Wait for motor to pick up speed when turning
    the motor on if it is currently off.
    Entry conditions:
    A contains the previous motor state.
    Exit conditions:
    If the motor was turned on or off OK:
    Carry true.
    If the user hit escape:
    Carry false.
    Always:
    A and other flags corrupt.
    All other registers preserved.
    Notes:
    This routine uses the previous motor state as returned by CAS START MOTOR or
    CAS STOP MOTOR.
    If calling this routine results in the motor being turned on when it is currently off then
    the routine waits for approximately two seconds to allow the tape to reach full speed.
    The motor is always turned on or off (as appropriate) by this routine. If the user hits
    the escape key then this merely truncates the time spent waiting for the motor to pick
    up speed.
    Related entries:
    CAS START MOTOR
    CAS STOP MOTOR
*/
uint8_t fw_cas_restore_motor(uint8_t previous_motor_state) __preserves_regs(b, c, d, e, h, iyh, iyl) __z88dk_fastcall;



/** This structure was introduced for use as a parameter to
    fw_cas_in_open().

    All in_* fields must be set.

    Return values will be written in out_* fields.
*/
typedef struct fw_cas_in_open_parameters_t
{
	/** filename does not have to be zero-terminated */
	unsigned char *in_filename;
	uint8_t in_filename_length;
	void *in_buffer;

	uint8_t out_filetype_or_error;
	void *out_data_location;
	uint16_t out_logical_file_length;
	void *out_header;
} fw_cas_in_open_parameters_t;


/** Two variants: tape and disc.

    #### CFWI-specific information: ####

    Use like this.


    There are many ways to initialize struct fields.  This one allows
    to have all fields set up with compile time constants (no code).
    But you can also fill them with code of course.

const unsigned char[] my_filename="mydata.bin";
enum 
{
my_filename_length = sizeof(my_filename)
	};
const uint8_t[2048] my_buffer;
FIXME
fw_cas_in_open_parameters_t params = 
{
in_filename=my_filename,
	in_filename_length=my_filename_length,
	in_buffer=my_buffer
	}
	
	uint8_t rc = fw_cas_in_open();
switch (rc)
{
case 2:
// user hit escape
break;
case 1:
// opened ok
break;
case 0:
// stream is in use
break;
}



    

    125: CAS IN OPEN
    #BC77
    Open a file for input.
    Action:
    Set up the read stream for reading a file and read the first block.
    Entry conditions:
    B contains the length of the filename.
    HL contains the address of the filename.
    DE contains the address of a 2K buffer to use.
    Exit conditions:
    If the file was opened OK:
    Carry true.
    Zero false.
    HL contains the address of a buffer containing the file header.
    DE contains the data location (from the header).
    BC contains the logical file length (from the header).
    A contains the file type (from the header).
    If the stream is in use:
    Carry false.
    Zero false.
    In V1.1: A contains an error number (#0E).
    In V1.0: A corrupt.
    BC,DE and HL corrupt.
    If the user hit escape:
    Carry false.
    Zero true.
    In V1.1: A contains an error number (#00).
    In V1.0: A corrupt.
    BC,DE and HL corrupt.
    Always:
    IX and other flags corrupt.
    All other registers preserved.


    125: CAS IN OPEN (DISC)
    #BC77
    Open a file for input.
    Action:
    Set up the read stream for reading a file and read the header if there is one, other wise
    create a fake header in store.
    Entry conditions:
    B contains the length of the filename.
    HL contains the address of the filename.
    DE contains the address of a 2K buffer to use.
    Exit conditions:
    If the file was opened OK:
    Carry true.
    Zero false.
    HL contains the address of a buffer containing the file header.
    DE contains the data location (from the header).
    BC contains the logical file length (from the header).
    A contains the file type (from the header).
    If the stream is already open:
    Carry false.
    Zero false.
    A contains an error number (#0E).
    BC,DE and HL corrupt.
    If the open failed for any other reason:
    Carry false.
    Zero true.
    A contains an error number.
    BC,DE and HL corrupt.
    Always:
    IX and other flags corrupt.
    All other registers preserved.
    Notes:
    The 2K buffer (2048 bytes) supplied is used to store the contents of a block of the file
    when it is read from disc. It will remain in use until the file is closed by calling either
    CAS IN CLOSE or CAS IN ABANDON. The buffer may lie anywhere in memory,
    even underneath a ROM.
    The filename must conform to the AMSDOS conventions with no wild cards. The
    filename may lie anywhere in RAM, even underneath a ROM.
    If the type part of the filename is omitted AMSDOS will attempt to open, in turn, a
    file with the following type parts '.', '.BAS', '.BIN'. If none of these exist then the
    open will fail.
    When the file is opened the first record of the file is read immediately. If this record
    contains a header then it is copied into store, otherwise a fake header is constructed in
    store. The address of the area where the header is stored is passed back to the user so
    that information can be extracted from it. This area will lie in the central 32K of
    RAM. The user is not allowed to write to the header, only read from it. AMSDOS uses
    fields in the header for its own purposes and so these may differ from those read from
    the disc. The file type, logical length, entry point and all user fields will remain
    unchanged.
    Related entries:
    CAS
    CAS
    CAS
    CAS
    CAS
    CAS
    IN ABANDON (DISC)
    IN CHAR (DISC)
    IN CLOSE (DISC)
    IN DIRECT (DISC)
    IN OPEN
    OUT OPEN (DISC)

*/
uint8_t fw_cas_in_open(fw_cas_in_open_parameters_t *parameters) __z88dk_fastcall __preserves_regs(iyh, iyl);

/** Two variants: tape and disc.

    126: CAS IN CLOSE
    #BC7A
    Close the input file properly.
    Action:
    Mark the read stream as closed.
    Entry conditions:
    No conditions.
    Exit conditions:
    If the stream was closed OK:
    Carry true.
    A corrupt.
    If the stream was not open:
    Carry false.
    In V1.1: A contains an error number (#0E).
    In V1.0: A corrupt.
    Always:
    BC, DE, HL and other flags corrupt.
    All other registers preserved.
    Notes:
    This routine can only return one error number:
    #0E:
    The stream is not open
    This routine should be called to close a file after reading from it using either CAS IN
    CHAR or CAS IN DIRECT.
    The user may reclaim the buffer passed to CAS IN OPEN after calling this routine.
    Related entries:
    CAS
    CAS
    CAS
    CAS
    IN ABANDON
    IN CLOSE (DISC)
    IN OPEN
    OUT CLOSE

    126: CAS IN CLOSE (DISC)
    #BC7A
    Close the input file properly.
    Action:
    Mark the read stream as closed.
    Entry conditions:
    No conditions.
    Exit conditions:
    If the stream was closed OK:
    Carry true.
    Zero false.
    A corrupt.
    If the stream is not open:
    Carry false.
    Zero false.
    A contains an error number (#0E).
    If the close failed for any other reason:
    Carry false.
    Zero true.
    A contains an error number.
    Always:
    BC, DE, HL and other flags corrupt.
    All other registers preserved.
    Notes:
    This routine should be called to close a file after reading from it using either CAS IN
    CHAR or CAS IN DIRECT.
    The user may reclaim the buffer passed to CAS IN OPEN after calling this routine.
    The drive motor is turned off immediately after the input file has closed. This is done
    so that a loaded program which takes over the machine is not left with the motor
    running indefinitely.
    Related entries:
    CAS
    CAS
    CAS
    CAS
    IN ABANDON (DISC)
    IN CLOSE
    IN OPEN (DISC)
    OUT CLOSE (DISC)

*/
uint8_t fw_cas_in_close(void) __preserves_regs(iyh, iyl);

#endif /* __FW_CAS_H__ */
