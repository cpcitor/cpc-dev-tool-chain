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


#endif /* __FW_CAS_H__ */
