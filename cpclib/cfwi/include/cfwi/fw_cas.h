#ifndef  __FW_CAS_H__
#define __FW_CAS_H__

#include <stdint.h>

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

/** Two variants: tape and disc.

    127: CAS IN ABANDON
    #BC7D
    Close the input file immediately.
    Action:
    Abandon reading from the read stream and close it.
    Entry conditions:
    No conditions.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    This routine is intended for use after an error or in similar circumstances.
    The user may reclaim the buffer passed to CAS IN OPEN after calling this routine.
    Related entries:
    CAS
    CAS
    CAS
    CAS
    IN ABANDON (DISC)
    IN CLOSE
    IN OPEN
    OUT ABANDON

    127: CAS IN ABANDON (DISC)
    #BC7D
    Close the input file immediately.
    Action:
    Abandon reading from the read stream and close it.
    Entry conditions:
    No conditions.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    This routine is intended for use after an error or in similar circumstances.
    The user may reclaim the buffer passed to CAS IN OPEN after calling this routine.
    Related entries:
    CAS
    CAS
    CAS
    CAS
    IN ABANDON
    IN CLOSE (DISC)
    IN OPEN (DISC)
    OUT ABANDON (DISC)
*/
void fw_cas_in_abandon(void) __preserves_regs(iyh, iyl);

/** Two variants: tape and disc.

    128: CAS IN CHAR
    #BC80
    Read a character from the input file.
    Action:
    Read a character from the input stream. Fetches blocks from tape as required.
    Entry conditions:
    No conditions.
    Exit conditions:
    If the character was read OK:
    Carry true.
    Zero false.
    A contains the character read from the file.
    If the end of the file was found:
    Carry false.
    Zero false.
    In V1.1: A contains an error number (#0E or #0F).
    In V1.0: A corrupt.
    If the user hit escape:
    Carry false.
    Zero true.
    In V1.1: A contains an error number (#00).
    In V1.0: A corrupt.
    Always:
    IX and other flags corrupt.
    All other registers preserved.
    Notes:
    This routine can return three error numbers:
    #00: The user hit escape.
    #0E: The stream is not open for reading characters or the user hit escape previously.
    #0F: Have reached the end of the file.
    Once the first character has been read from a file it can only be used for character by
    character access. It is not possible to switch to direct reading (by CAS IN DIRECT).
    Related entries:
    CAS
    CAS
    CAS
    CAS
    CAS
    CAS
    CAS
    IN CHAR (DISC)
    IN CLOSE
    IN DIRECT
    IN OPEN
    OUT CHAR
    RETURN
    TEST EOF


    128: CAS IN CHAR (DISC)
    #BC80
    Read a character from an input file.
    Action:
    Read a character from the input stream.
    Entry conditions:
    No conditions.
    Exit conditions:
    If the character was read OK:
    Carry true.
    Zero false.
    A contains the character read from the file.
    If the end of the file was found, or stream not open as expected:
    Carry false.
    Zero false.
    A contains an error number (#0E, #0F or #1A).
    If failed for any other reason:
    Carry false.
    Zero true.
    A contains an error number.
    Always:
    IX and other flags corrupt.
    All other registers preserved.
    Notes:
    Once the character has been read from a file the rest of the file may only be read
    character by character (using CAS IN CHAR). It is impossible to switch to direct
    reading (by CAS IN DIRECT).
    The CP/M end of file character (#1A) is treated as end of file (carry false, zero false).
    However, it is possible to continue reading characters until the hard end of file. The
    error number returned is set to #1A for soft (CP/M) end of file and #0F for hard end
    of file. The action for spotting soft and of file is not performed by the equivalent
    cassette version of the routine and will never return #1A when carry is false.
    If a file containing binary data is read using this routine then it will be necessary to
    spot soft EOF and ignore it.
    Related entries:
    CAS
    CAS
    CAS
    CAS
    IN
    IN
    IN
    IN
    CHAR
    CLOSE (DISC)
    DIRECT (DISC)
    OPEN (DISC)
    CAS OUT CHAR (DISC)
    CAS RETURN (DISC)
    CAS TEST EOF (DISC)


*/
// TODO write decode union
uint16_t fw_cas_in_char(void) __preserves_regs(b, c, d, e, iyh, iyl);

/** Two variants: tape and disc.

    129: CAS IN DIRECT
    #BC83
    Read the input file into store.
    Action:
    Read the input file directly into store in one go rather than one character at a time.
    Entry conditions:
    HL contains the address to put the file (anywhere in RAM).
    Exit conditions:
    If the file was read OK:
    Carry true.
    Zero false.
    HL contains the entry address (from the header).
    A corrupt.
    If the file was not open as expected:
    Carry false.
    Zero false.
    In V1.1: A contains an error number (#0E).
    In V1.0: A corrupt.
    HL corrupt.
    If the user hit escape:
    Carry false.
    Zero true.
    In V1.1: A contains an error number (#00).
    In V1.0: A corrupt.
    HL corrupt.
    Always:
    BC, DE, IX and other flags corrupt.
    All other registers preserved.
    Notes:
    This routine can return two error numbers:
    #00: The user hit escape.
    #0E: The stream is not open for reading directly or the user hit escape previously.
    The read stream must be newly opened (by CAS IN OPEN). If the stream has been
    used for character access (by calling CAS IN CHAR) then it is not possible to directly
    read the file. Neither is it possible to directly read from the file more than once. This
    will merely corrupt the copy of the file read.
    The buffer of data read when the stream was opened is copied to its correct position
    and the remainder of the file (if any) is also read into store.
    CAS IN DIRECT (DISC)
    Related entries:
    CAS IN CHAR
    CAS IN OPEN
    CAS IN CLOSE
    CAS OUT DIRECT

    129: CAS IN DIRECT (DISC)
    #BC83
    Read the input file into store.
    Action:
    Read the input file directly into store in one go rather than one character at a time.
    Entry conditions:
    HL contains the address to put the file (anywhere in RAM).
    Exit conditions:
    If the file was read OK:
    Carry true.
    Zero false.
    HL contains the entry address (from the header).
    A corrupt.
    If the stream is not open as expected:
    Carry false.
    Zero false.
    A contains an error number (#0E).
    HL corrupt.
    If the read failed for any other reason:
    Carry false.
    Zero true.
    A contains an error number.
    HL corrupt.
    Always:
    BC, DE, IX and other flags corrupt.
    All other registers preserved.
    Notes:
    The read stream must be newly opened (by CAS IN OPEN). If the stream has been
    used for character access (by calling CAS IN CHAR or CAS TEST EOF) then it is not
    possible to directly read the file. Neither is it possible to directly read from the file
    more than once. (Any attempt to do so will corrupt the copy of the file read.)
    If the file has a header then the number of bytes read is that recorded in the 24 bit file
    length field (bytes 64..66 of the disc file header). If there is no header the file is read
    until hard end of file.
    The CP/M end of file character, #1A, is not treated as end of file.
    Related entries:
    CAS IN CHAR (DISC)
    CAS IN CLOSE (DISC)
    CAS IN DIRECT
    CAS IN OPEN (DISC)
    CAS OUT DIRECT (DISC)
*/
// TODO write decode union
uint32_t fw_cas_in_direct(void *destination_buffer) __preserves_regs(iyh, iyl);

/** Two variants: tape and disc.

    130: CAS RETURN
    #BC86
    Put the last character read back.
    Action:
    Put the last character read by CAS IN CHAR back into the read buffer. The character
    will be re-read next time CAS IN CHAR is called.
    Entry conditions:
    No conditions.
    Exit conditions:
    All registers and flags preserved.
    Notes:
    It is only possible to use this routine to return the last character that has been read by
    CAS IN CHAR. At least one character must have been read since:
    or
    or
    the stream was opened
    the last character was returned
    the last test for end of file was made.
    Related entries:
    CAS IN CHAR
    CAS RETURN (DISC)

    130: CAS RETURN (DISC)
    #BC86
    Put the last character read back.
    Action:
    Put the last character read by CAS IN CHAR back into the read buffer. The character
    will be re-read next time CAS IN CHAR is called.
    Entry conditions:
    No conditions.
    Exit conditions:
    All registers and flags preserved.
    Notes:
    It is only possible to use this routine to return the last character that has been read by
    CAS IN CHAR. At least one character must have been read since:
    or
    or
    the stream was opened
    the last character was returned
    the last test for end of file was made.
    Related entries:
    CAS IN CHAR (DISC)
    CAS RETURN
*/
void fw_cas_return(void) __preserves_regs(a, b, c, d, e, h, l, iyh, iyl);

/** Two variants: tape and disc.

    131: CAS TEST EOF
    #BC89
    Have we reached the end of the input file yet?
    Action:
    Test if the end of the input file has been reached.
    Entry conditions:
    No conditions.
    Exit conditions:
    If the end of the file was not found:
    Carry true.
    Zero false.
    A corrupt.
    If the end of the file was found:
    Carry false.
    Zero false.
    In V1.1: A contains an error number (#0E or #0F).
    In V1.0: A corrupt.
    If the user hit escape:
    Carry false.
    Zero true.
    In V1.1: A contains an error number (#00).
    In V1.0: A corrupt.
    Always:
    IX and other flags corrupt.
    All other registers preserved.
    Notes:
    This routine can return three error numbers:
    #00: The user hit escape.
    #0E: The stream is not open for reading characters or the user hit escape previously.
    #0F: Have reached the end of the file.
    Calling this routine put the stream into character input mode. It is not possible to use
    direct reading after calling this routine.
    It is not possible to call CAS RETURN after this routine has been called. A character
    must be read first.
    Related entries:
    CAS IN CHAR
    CAS TEST EOF (DISC)

    131: CAS TEST EOF (DISC)
    #BC89
    Have we reached the end of the input file yet?
    Action:
    Test if the end of the input file has been reached.
    Entry conditions:
    No conditions.
    Exit conditions:
    If the end of the file was not found:
    Carry true.
    Zero false.
    A corrupt.
    If the end of the file was found or stream was not open as expected:
    Carry false.
    Zero false.
    A contains an error number (#0E,#0F or #1A).
    If failed for any other reason:
    Carry false.
    Zero true.
    A contains an error number.
    Always:
    IX and other flags corrupt.
    All other registers preserved.
    Notes:
    This routine will report end of file if either there are no more characters in the file or
    if the next character to be read is the CP/M end of file character, #1A.
    Calling this routine puts the stream into character input mode. It is not possible to use
    direct reading after calling this routine.
    It is not possible to call CAS RETURN after this routine has been called. A character
    must be read first.
    Related entries:
    CAS IN CHAR (DISC)
    CAS TEST EOF
*/
uint16_t fw_cas_test_eof(void) __preserves_regs(b, c, d, e, iyh, iyl);


/** This structure was introduced for use as a parameter to
    fw_cas_out_open().

    All in_* fields must be set.

    Return values will be written in out_* fields.
*/
typedef struct fw_cas_out_open_parameters_t
{
	/** filename does not have to be zero-terminated */
	unsigned char *in_filename;
	uint8_t in_filename_length;
	void *in_buffer;

	void *out_header;
} fw_cas_out_open_parameters_t;

/** Two variants: tape and disc.

    132: CAS OUT OPEN
    #BC8C
    Open a file for output.
    Action:
    Set up the write stream for output.
    Entry conditions:
    B contains the length of the filename.
    HL contains the address of the filename.
    DE contains the address of a 2K buffer to use.
    Exit conditions:
    If the user hit escape:
    Carry false.
    Zero true.
    In V1.1: A contains an error number (#00).
    In V1.0: A corrupt.
    HL corrupt.
    If the stream is in use already:
    Carry false.
    Zero false.
    In V1.1: A contains an error number (#0E).
    In V1.0: A corrupt.
    HL corrupt.
    If the file was opened OK:
    Carry true.
    Zero false.
    HL contains the address of a buffer containing the header that will be written to
    each file block.
    Always:
    BC,DE,IX and other flags corrupt.
    All other registers preserved.
    Notes:
    This routine can only return two error numbers.
    #00: The user hit escape.
    #0E: The stream is already open.
    When writing files character by character the 2K buffer (2048 bytes) supplied is used
    to store the contents of a block of the file before it is written to tape. It will remain in
    use until the file is closed by calling either CAS OUT CLOSE or CAS OUT
    ABANDON. The buffer may reside anywhere in memory - even underneath a ROM.

    When the stream is opened for writing, a header is set up which will be written at the
    start of each block of the file. Many of the fields in the header are set by the Cassette
    Manager but the remainder are available for use by the user. The address of this
    header is passed to the user so that information can be stored in it. The user may write
    to the file type, logical length, entry point and all user fields. The user is not allowed
    to write to any other field in the header. The user settable fields are all zeroized
    initially, with the exception of the file type which is set to unprotected ASCII version
    1. (See section 8.4 for a description of the header).
    The filename passed is copied into the write stream descriptor. If it is longer than 16
    characters then it is truncated to 16 characters. If it is shorter than 16 characters then
    it is padded with nulls (#00) to 16 characters. While the filename may contain any
    character, it is best to avoid nulls. Lower case ASCII letters (characters #61..#7A) are
    converted to their upper case equivalents (characters #41..#5A). The filename may lie
    anywhere in RAM, even underneath a ROM.
    Related entries:
    CAS
    CAS
    CAS
    CAS
    CAS
    CAS
    IN OPEN
    OUT ABANDON
    OUT CHAR
    OUT CLOSE
    OUT DIRECT
    OUT OPEN (DISC)


    132: CAS OUT OPEN (DISC)
    #BC8C
    Open a file for output.
    Action:
    Set up the write stream for output.
    Entry conditions:
    B contains the length of the filename.
    HL contains the address of the filename.
    DE contains the address of a 2K buffer to use.
    Exit conditions:
    If the file was opened OK:
    Carry true.
    Zero false.
    HL contains the address of the buffer containing the header.
    A corrupt.
    If the stream is open already:
    Carry false.
    Zero false.
    A contains an error number (#0E).
    HL corrupt.
    If the open failed for any other reason:
    Carry false.
    Zero true.
    A contains an error number.
    HL corrupt.
    Always:
    BC,DE,IX and other flags corrupt.
    All other registers preserved.
    Notes:
    When characters are output to the file using CAS OUT CHAR the 2K buffer supplied
    is used by AMSDOS to buffer the output. It will remain in use until the file is closed
    by calling either CAS OUT CLOSE or CAS OUT ABANDON. The buffer may reside
    anywhere in memory - even underneath a ROM.

    The filename passed must conform to AMSDOS conventions with no wild cards. It is
    copied into the write stream header. The filename my lie anywhere in RAM - even
    underneath a ROM.
    The file is opened with a type part of '.$$$' regardless of the type part supplied. Any
    existing file with the same name and type part of '.$$$' is deleted. The file is renamed
    to its supplied name when CAS OUT CLOSE is called.
    When the stream is opened aa header is set up. Many of the fields in the header are set
    by AMSDOS but the remainder are available for use by the user. The address of this
    header is passed to the user so the information can be stored in it. The user may write
    to the file type, logical length, entry point and all other fields. The user is not allowed
    to write to any other field of the header. The user settable fields are all zeroized
    initially, with the exception of the file type which is set to unprotected ASCII version
    1.
    The header type field must be written to before CAS OUT CHAR or CAS OUT
    DIRECT is called. The type field must not be altered after calling either of these
    routines. If the file type is set to any type other than unprotected ASCII then space
    will be preserved for the header which when the file is closed.
    Related entries:
    CAS
    CAS
    CAS
    CAS
    CAS
    CAS
    IN OPEN (DISC)
    OUT ABANDON (DISC)
    OUT CHAR (DISC)
    OUT CLOSE (DISC)
    OUT DIRECT (DISC)
    OUT OPEN
*/
// TODO complicated needs input struct void fw_cas_out_open() __preserves_regs(b, c, d, e, iyh, iyl);
uint8_t fw_cas_out_open(fw_cas_out_open_parameters_t *parameters) __z88dk_fastcall __preserves_regs(iyh, iyl);

/** Two variants: tape and disc.

    133: CAS OUT CLOSE
    #BC8F
    Close the output file properly.
    Action:
    Mark the write stream as closed and write the last buffer area of data to tape.
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
    In V1.1: A contains an error number (#0E).
    In V1.0: A corrupt.
    Always:
    BC, DE, HL, IX and other flags corrupt.
    All other registers preserved.
    Notes:
    This routine can return two error numbers:
    #00: The user hit escape.
    #0E: The stream is not open.
    It is necessary to call this routine after using CAS OUT CHAR or CAS OUT DIRECT
    to cause the last block of data to be written to the tape. If the block is zero bytes long
    (nothing has been written to the file) then nothing is written to tape.
    If writing is to be abandoned then CAS OUT OPEN should be called as this does not
    write the last block of data to the tape.
    If the user hits escape during the writing of the last block then the file is left open and
    is not closed.
    The user may reclaim the buffer passed to CAS OUT OPEN after calling this routine.
    Related entries:
    CAS
    CAS
    CAS
    CAS
    IN CLOSE
    OUT ABANDON
    OUT CLOSE (DISC)
    OUT OPEN


    133: CAS OUT CLOSE (DISC)
    #BC8F
    Close the output file properly.
    Action:
    Mark the write stream as closed and give it its correct name.
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
    BC, DE, HL, IX and other flags corrupt.
    All other registers preserved.
    Notes:
    It is necessary to call this routine after using CAS OUT CHAR or CAS OUT DIRECT
    to ensure that all the data is written to the disc, to write the header to the start of the
    file and to give the file its true name.
    If no data has been written to the file then it is abandoned and nothing is written to
    disc. This is for compatability with cassette routines.

    When the file was opened it was given the type part of '.$$$'. This routine will rename
    the file to its true name and rename any existing version to have a '.BAK' type part.
    This ensures that any previous version of the file is automatically kept as a backup.
    Any existing '.BAK' version is deleted. If, when the file was opened, the caller did not
    specify the type part then AMSDOS will use the type part '.BAS' for BASIC files,
    '.BIN' for binary files and '. ' for all other files, as specified by the file type field in
    the header.
    If the actual length of the file is not a multiple of 128 bytes (a CP/M record) then a
    CP/M end of file character, #1A, is added to the file. This additional character is not
    recorded in the length of the file.
    If writing is to be abandoned then CAS OUT OPEN should be called as this does not
    write any more data to disc.
    The user may reclaim the buffer passed to CAS OUT OPEN after calling this routine.
    Related entries:
    CAS
    CAS
    CAS
    CAS
    IN CLOSE (DISC)
    OUT ABANDON (DISC)
    OUT CLOSE
    OUT OPEN (DISC)

*/
// TODO write decode union
uint16_t fw_cas_out_close(void) __preserves_regs(iyh, iyl);


/** Two variants: tape and disc.

    134: CAS OUT ABANDON
    #BC92
    Close the output file immediately.
    Action:
    Abandon the output file and mark the write stream closed. Any unwritten data is
    discarded and not written to tape.
    Entry conditions:
    No conditions.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    This routine in intended for use after an error or in similar circumstances.
    Related entries:
    CAS
    CAS
    CAS
    CAS
    IN ABANDON
    OUT ABANDON (DISC)
    OUT CLOSE
    OUT OPEN

    134: CAS OUT ABANDON (DISC)
    #BC92
    Close the output file immediately.
    Action:
    Abandon the output file and mark the write stream closed. Any unwritten data is
    discarded and not written to disc.
    Entry conditions:
    No conditions.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    This routine is intended for use after an error or similar circumstances.
    If more than one 16K physical extent has already been written to disc then the file
    will appear in the disc directory with a type part of '.$$$'. Otherwise the file will
    disappear. This is because each 16K of a file requires a directory entry. A directory
    entry is not written to disc until the 16K has been written or a file is closed (CAS
    OUT CLOSE).
    Related entries:
    CAS
    CAS
    CAS
    CAS
    IN ABANDON (DISC)
    OUT ABANDON
    OUT CLOSE (DISC)
    OUT OPEN (DISC)

*/
void fw_cas_out_abandon(void) __preserves_regs(iyh, iyl);

/** Two variants: tape and disc.

    135: CAS OUT CHAR
    #BC95
    Write a character to the output file.
    Action:
    Add a character to the buffer for the write stream. If the buffer is already full then it
    is written to tape before the new character is inserted.
    Entry conditions:
    A contains the character to write.
    Exit conditions:
    If the character was written OK:
    Carry true.
    Zero false.
    A corrupt.
    If the file was not open as expected:
    Carry false.
    Zero false.
    In V1.1: A contains an error number (#0E).
    In V1.0: A corrupt.
    If the user hit escape:
    Carry false.
    Zero true.
    In V1.1: A contains an error number (#00).
    In V1.0: A corrupt.
    Always:
    A, IX and other flags corrupt.
    All other registers preserved.
    Notes:
    This routine can return two error numbers:
    #00: The user hit escape.
    #0E: The stream is not open for writing characters or the user hit escape previously.
    If this routine returns the file not open as expected condition then either the user has
    hit escape previously or the file has been written using CAS OUT DIRECT. In either
    case, or if escape is pressed, the character sent will be discarded.
    It is necessary to call CAS OUT CLOSE after sending all the characters to the file to
    ensure that the last block is written to the tape.
    Once the routine has been called it is not possible to switch to directly writing the
    file.
    Related entries:
    CAS IN CHAR
    CAS OUT CHAR (DISC)
    CAS OUT CLOSE
    CAS OUT DIRECT
    CAS OUT OPEN

    135: CAS OUT CHAR (DISC)
    #BC95
    Write a character to an output file.
    Action:
    Add a character to the buffer for the write stream. If the buffer is already full then it
    is written to disc before the new character is inserted.
    Entry conditions:
    A contains the character to write.
    Exit conditions:
    If the character was written OK:
    Carry true.
    Zero false.
    A corrupt.
    If the stream is not open as expected:
    Carry false.
    Zero false.
    A contains an error number (#0E).
    If failed for any other reason:
    Carry false.
    Zero true.
    A contains an error number.
    Always:
    IX and other flags corrupt.
    All other registers preserved.
    Notes:
    It is necessary to call CAS OUT CLOSE after sending all the characters to the file to
    ensure that the file is correctly written to disc.
    Once the routine has been called it is not possible to switch to directly writing the file
    (CAS OUT DIRECT).
    Related entries:
    CAS IN CHAR (DISC)
    CAS OUT CHAR
    CAS OUT CLOSE (DISC)
    CAS OUT DIRECT (DISC)
    CAS OUT OPEN (DISC)

*/
// TODO write decode union
uint16_t fw_cas_out_char(unsigned char char_to_write) __preserves_regs(iyh, iyl);

/** Two variants: tape and disc.

    136: CAS OUT DIRECT
    #BC98
    Write the output file directly from store.
    Action:
    Write the contents of store directly out to the output file.
    Entry conditions:
    HL contains the address of the data to write.
    DE contains the length of the data to write.
    BC contains the entry address (to go into the header).
    A contains the file type (to go into the header).
    Exit conditions:
    If the file was written OK:
    Carry true.
    Zero false.
    A corrupt.
    If the file was not open as expected:
    Carry false.
    Zero false.
    In V1.1: A contains an error number (#0E).
    In V1.0: a corrupt.
    If the user hit escape:
    Carry false.
    Zero true.
    In V1.1: A contains an error number (#00).
    In V1.0: A corrupt.
    Always:
    BC, DE, HL, IX and other flags corrupt.
    All other registers preserved.
    Notes:
    This routine can return two error numbers:
    #00: The user hit escape.
    #0E: The stream is not newly opened.
    After writing the file it must be closed using CAS OUT CLOSE to ensure that the last
    block of the file is written to tape.
    It is not possible to change the method for writing files from character output (using
    CAS OUT CHAR) to direct output (using CAS OUT DIRECT) or visa versa once the
    method has been chosen. Nor is it possible to directly write a file in two of more parts
    by calling CAS OUT DIRECT more than once - this will write corrupt data.
    Attempting to break these rules will result in a file not open as expected error.
    Related entries:
    CAS IN DIRECT
    CAS OUT CLOSE
    CAS OUT DIRECT (DISC)
    CAS OUT OPEN

    136: CAS OUT DIRECT (DISC)
    #BC98
    Write the output file directly from store.
    Action:
    Write the contents of store directly out to the output file.
    Entry conditions:
    HL contains the address of the data to write (to go into the header).
    DE contains the length of the data to write (to go into the header).
    BC contains the entry address (to go into the header).
    A contains the file type (to go into the header).
    Exit conditions:
    If the file was written OK:
    Carry true.
    Zero false.
    A corrupt.
    If the stream is not open as expected:
    Carry false.
    Zero false.
    A contains an error number (#0E).
    If failed for any other reason:
    Carry false.
    Zero true.
    A contains an error number.
    Always:
    BC, DE, HL, IX and other flags corrupt.
    All other registers preserved.
    Notes:
    After writing the file it must be closed using CAS OUT CLOSE to ensure that the file
    is written to disc.
    It is not possible to change the method for writing files from character output (using
    CAS OUT CHAR) to direct output (using CAS OUT DIRECT) or visa versa once the
    method has been chosen. Nor is it possible to directly write a file in two of more parts
    by calling CAS OUT DIRECT more than once - this will write corrupt data.
    Related entries:
    CAS
    CAS
    CAS
    CAS
    IN DIRECT (DISC)
    OUT CLOSE (DISC)
    OUT DIRECT
    OUT OPEN (DISC)

*/
// TODO Complicated void fw_cas_out_direct(void);


/** Two variants: tape and disc.

    137: CAS CATALOG
    #BC9B
    Generate a catalogue from tape.
    Action:
    Read file blocks to check their validity and print information about them on the
    screen.
    Entry conditions:
    DE contains the address of a 2K buffer to use.
    Exit conditions:
    If the cataloguing went OK:
    Carry true.
    Zero false.
    A corrupt.
    If the read stream was in use:
    Carry false.
    Zero false.
    In V1.1: A contains an error number (#0E).
    In V1.0: A corrupt.
    Always:
    BC, DE, HL, IX and other flags corrupt.
    All registers preserved.
    Notes:
    This routine can only return one error number:
    #0E: The stream is already in use.
    This routine uses the read stream and so the stream must be closed when it is called.
    The read stream remains closed when this routine exits. The write stream is
    unaffected by this routine.

    The prompt messages are turned on (see CAS NOISY) by this routine.
    When cataloguing the Cassette Manager reads a header record, prints information
    from it and then reads the data record. This cycle repeats until the user hits the escape
    key. The information printed is as follows:
    FILENAME block N T Ok
    FILENAME is the name of the file on the tape, or 'Unnamed file' if the filename starts
    with a null (character #00).
    N is the number of the block. Block 1 is normally the first block in a file.
    T is a representation of the file type of the file. It is formed by adding #24 (the
    character '$') to the file type byte masked with #0F (to remove the version number
    field). The standard file types are thus:
    $
    %
    *
    &
    ‘
    a BASIC program file
    a protected BASIC program file
    an ASCII text file (default file type)
    a binary file.
    a protected binary file
    Other file types are possible but will not have been written by the BASIC in the on-
    board ROM. See section 8.4 for a description of the file type byte.
    Ok is printed after the end of the data record. This shows that the data was read
    without errors and also serves to indicate the end of the data on tape (to help avoid
    over-recording a tape file).
    Related entries:
    CAS CATALOG (DISC)
    CAS NOISY

    137: CAS CATALOG (DISC)
    #BC9B
    Display the disc directory
    Action:
    Display the disc directory for the current drive and current user. The directory is
    sorted into alphabetical order and displayed in as many columns as will fit in the
    current text window (stream #0). The size in Kbytes is shown along side each file.
    The total amount of free space on the disc is also shown.
    Entry conditions:
    DE contains the address of a 2K buffer to use.
    Exit conditions:
    If the cataloguing went OK:
    Carry true.
    Zero false.
    A corrupt.
    If failed for any reason:
    Carry false.
    Zero true.
    A contains an error number.
    Always:
    BC, DE, HL, IX and other flags corrupt.
    All registers preserved.
    Notes:
    Files marked SYS are not shown.
    Files marked R/O are shown with a '*' after the filename.
    Unlike the cassette version of this routine, the disc input stream is not required.
    (Note: BASIC abandons both the input and output streams when generating the
    catalogue.)
    Related entries:
    CAS CATALOG
    |DIR
*/
uint8_t fw_cas_catalog(void) __preserves_regs(iyh, iyl);

/** 138: CAS WRITE
    #BC9E
    Write a record to tape.
    Action:
    Write a record to the cassette. This routine is used by the higher level routines (CAS
    OUT CHAR, CAS OUT DIRECT and CAS OUT CLOSE) to write the header and data
    records that make up a tape file.
    Entry conditions:
    HL contains the address of the data to write.
    DE contains the length of the data to write.
    A contains the sync character to write at the end of the leader.
    Exit conditions:
    If the record was written OK:
    Carry true.
    A corrupt.
    If an error occurred or the user hit escape:
    Carry false.
    A contains an error code.
    Always:
    BC, DE, HL, IX corrupt.
    All other registers preserved.
    Notes:
    A data length of 0 passed to this routine is taken to mean 65536 bytes and all of the
    memory will be written to tape. (This is unlikely to be useful).
    The data to be written may lie anywhere in RAM, even underneath a ROM.
    The sync character is used to distinguish header records (sync is #2C) from data
    records (sync is #16). Other sync characters could be used but the resulting record
    would require special action to be taken to read it.
    The error codes returned by this routine are:
    0	Break	The user hit the escape key.
    1	Overrun	The Cassette Manager was unable to get back to writing a bit fast enough.

    Because reading and writing the tape requires stringent timing considerations
    interrupts are disabled whilst the tape is being written (potentially a period of over 5
    minutes). It would be unpleasant to have the sound chip making a noise for all this
    time so the Sound Manager is shut down (SOUND RESET). When writing to the tape
    has finished interrupts are re-enabled.
    The cassette motor is started by this routine (in case it is not already on) and restored
    to its previous state when writing is completed.
    Related entries:
    CAS CHECK
    CAS READ

*/
// TODO complicated struct entry void fw_cas_write(void);

#endif /* __FW_CAS_H__ */
