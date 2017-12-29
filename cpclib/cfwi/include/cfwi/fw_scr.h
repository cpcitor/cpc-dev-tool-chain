#ifndef __FW_SCR_H__
#define __FW_SCR_H__

#include <stdint.h>

/**
   85: SCR INITIALISE
   #BBFF
   Initialize the Screen Pack.
   Action:
   Full initialization of the Screen Pack (as used during EMS). All Screen Pack variables
   and indirections are initialized, also the screen mode and the inks are initialized to
   their default settings.
   Entry conditions:
   No conditions.
   Exit conditions:
   AF, BC, DE and HL corrupt.
   All other registers preserved.
   Notes:
   The screen indirections (SCR READ, SCR WRITE and SCR MODE CLEAR) are set
   to their default routines.
   The inks are set to their default colours (see Appendix V).
   The ink flashing periods are set to their default values.
   The screen is put into mode 1.
   The screen base is set to put the screen memory at #C000..#FFFF (under the upper
   ROM.
   The screen offset is set to 0.
   The screen is cleared to ink 0.
   The Graphics VDU write mode is set to FORCE mode.
   The ink flashing frame flyback event is set up.
   The initialization is performed in an order that attempts to avoid the previous contents
   of the screen becoming visible (at EMS the contents will be random).
   Related entries:
   GRA INITIALISE
   SCR RESET
   TXT INITIALISE
*/
void fw_scr_initialise(void) __preserves_regs(iyh, iyl);

/** 86: SCR RESET
    #BC02
    Reset the Screen Pack.
    Action:
    Reinitializes the Screen Pack indirections and the ink colours. Also reinitializes the
    flash rate and Graphics VDU write mode.
    Entry conditions:
    No conditions.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    The screen indirections (SCR READ, SCR WRITE and SCR MODE CLEAR) are set
    to their default routines.
    The inks are set to their default colours (see Appendix V).
    The ink flashing periods are set to their default values.
    The Graphics VDU write mode is set to FORCE mode.
    The inks are not passed to the hardware. This will be done when the inks flash next.
    Related entries:
    SCR
    SCR
    SCR
    SCR
    INITIALISE
    SET ACCESS
    SET FLASHING
    SET INK
*/
void fw_scr_reset(void) __preserves_regs(iyh, iyl);

/** 87: SCR SET OFFSET
    #BC05
    Set the offset of the start of the screen.
    Action:
    Set the offset of the first character on the screen. By changing this offset the screen
    can be rolled.
    Entry conditions:
    HL contains the required offset.
    Exit conditions:
    AF and HL corrupt.
    All other registers preserved.
    Notes:
    The offset passed is masked with #07FE to make sure it is not too big to make sure
    that the offset is even. (The screen is only capable of rolling in two byte increments).
    The screen base and screen offset are combined into a single value and sent to the
    hardware together.
    The screen offset is used by SCR CHAR POSITION and SCR DOT POSITION to
    calculate screen addresses. If the screen offset is changed merely by calling the
    Machine Pack routine MC SCREEN OFFSET then the Text and Graphics VDUs will
    use incorrect screen addresses.
    The offset is set to zero when the screen mode is set or the screen is cleared by calling
    SCR CLEAR.
    Related entries:
    MC SCREEN OFFSET
    SCR GET LOCATION
    SCR HW ROLL
    SCR SET BASE
    SCR SET POSITION
*/
void fw_scr_set_offset(uint16_t offset) __z88dk_fastcall;

void fw_scr_set_mode(unsigned char x);

void fw_scr_clear(void);
void fw_scr_mode_clear(void);
void fw_scr_set_ink( uint8_t pen, uint8_t color1, uint8_t color2 );
void fw_scr_set_border( uint8_t color1, uint8_t color2 );

#endif /* __FW_SCR_H__ */
