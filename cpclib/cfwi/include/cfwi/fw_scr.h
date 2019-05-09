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
    SCR INITIALISE
    SCR SET ACCESS
    SCR SET FLASHING
    SCR SET INK
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
void fw_scr_set_offset(uint16_t offset) __z88dk_fastcall __preserves_regs(b, c, d, e, iyh, iyl);

/** 88: SCR SET BASE
    #BC08
    Set the area of RAM to use for the screen memory.
    Action:
    Set the base address of the screen memory. This can be used to move the screen out
    from underneath the upper ROM or to display a prepared screen instantly.
    Entry conditions:
    A contains the more significant byte of the base address.
    Exit conditions:
    AF and HL corrupt.
    All other registers preserved.
    Notes:
    The screen memory can only be located on a 16K boundary so the value passed in
    masked with #C0. The default screen base, set at EMS, is #C0.
    The screen offset is combined with the screen base into a single value which is sent to
    the hardware.
    The screen base address is used by SCR CHAR POSITION and SCR DOT POSITION
    to calculate screen addresses. If the screen base is changed merely by calling the
    Machine Pack routine MC SCREEN OFFSET then the text and graphics VDUs will
    use incorrect screen addresses.
    The screen memory is not cleared when the screen base is set, use SCR CLEAR to do
    this.
    Related entries:
    MC SCREEN OFFSET
    SCR GET LOCATION
    SCR SET OFFSET
    SCR SET POSITION
*/
void fw_scr_set_base( uint8_t base_msb) __z88dk_fastcall __preserves_regs(b, c, d, e, iyh, iyl);

/** 90: SCR SET MODE
    #BC0E
    Set screen into a new mode.
    Action:
    Put the screen into a new mode and make sure that the Text and Graphics VDUs are
    set up correctly.
    Entry conditions:
    A contains the required mode.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    The mode requested is masked with #03. If the resulting value is 3 then no action is
    taken. Otherwise one of the following screen modes is set up:
    Mode 0:
    Mode 1:
    Mode 2:
    160 x 200 pixels,
    320 x 200 pixels,
    640 x 200 pixels,
    20 x 25 characters.
    40 x 25 characters.
    80 x 25 characters.
    At an early stage the screen is cleared to avoid the old contents of the screen being
    displayed in the wrong mode. The screen is cleared by calling the SCR MODE
    CLEAR indirection.
    All the text and graphics windows are set to cover the whole screen and the graphics
    user origin is set to the bottom left corner of the screen. The cursor blobs for all text
    streams are turned off. Stream zero is selected.
    The current text and graphics pen and paper inks are masked as appropriate for the
    new mode (see TXT SET PEN et al). When changing mode to a mode that allows
    fewer inks on the screen this may cause the pen and paper inks to change.
    Related entries:
    MC SET MODE
    SCR GET MODE
 */
void fw_scr_set_mode(unsigned char x) __preserves_regs(iyh, iyl);

/* 92: SCR CLEAR
    #BC14
    Clear the screen (to ink zero).
    Action:
    Clear the whole of screen memory to zero.
    Entry conditions:
    No conditions.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    At an early stage the ink flashing is turned off and the inks are all set to the same
    colour as ink 0. This makes the screen clearing appear instantaneous. When all the
    screen memory has been set to 0 the ink flashing is turned back on (an ink flashing
    event is added to the frame flyback queue) and all inks are set to their proper colours.
    If the text paper ink and graphics paper ink are not set to ink 0 this will become
    apparent on the screen when characters are written or windows are cleared.
    The screen offset is set to zero.
    Related entries:
    GRA CLEAR WINDOW
    SCR MODE CLEAR
    TXT CLEAR WINDOW
*/
void fw_scr_clear(void) __preserves_regs(iyh, iyl);

// void fw_scr_mode_clear(void);

/*
102: SCR SET INK
#BC32
Set the colours in which to display an ink.
Action:
Set which two colours will be used to display an ink. If the two colours are the same
then the ink will remain a steady colour. If the colours are different then the ink will
alternate between these two colours.
Entry conditions:
A contains an ink number.
B contains the first colour.
C contains the second colour.
Exit conditions:
AF, BC, DE and HL corrupt.
All other registers preserved.
Notes:
The ink number is masked with #0F to make sure it is legal, and the colours are
masked with #1F. Colours 27..31 are not intended for use; they are merely duplicates
of other colours available.
The new colours for an ink are not sent to the hardware immediately. They are stored
and will appear on the screen when the next frame flyback occurs.
The length of time for which each colour is displayed on the screen can be set by
calling SCR SET FLASHING.
The inks are set to their default colours at EMS and when SCR RESET is called.
The various colours available and the default ink colours set are described in
Appendix V.
Related entries:
GRA SET PAPER
GRA SET PEN
SCR GET INK
SCR SET BORDER
SCR SET FLASHING
TXT SET PAPER
TXT SET PEN
*/
void fw_scr_set_ink( uint8_t pen, uint8_t color1, uint8_t color2 );

/** 104: SCR SET BORDER
    #BC38
    Set the colours in which to display the border.
    Action:
    Set which two colours will be used to display the border. If the two colours are the
    same then the border will remain a steady colour. If the colours are different then the
    border will alternate between these two colours.
    Entry conditions:
    B contains the first colour.
    C contains the second colour.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    The colours are masked with #1F to ensure that they are legal. Colours 27..31 are not
    intended for use; they are merely duplicates of other colours available.
    The new colours for the border are not sent to the hardware immediately. They are
    stored and will appear on the screen when the next frame flyback occurs.
    The length of time for which each colour is displayed on the screen can be set by
    calling SCR SET FLASHING.
    The border is set to its default colours at EMS and when SCR RESET is called. The
    default colour and the colours available are described in Appendix V.
    Related entries:
    SCR GET BORDER
    SCR SET FLASHING
    SCR SET INK
 */
void fw_scr_set_border( uint8_t color1, uint8_t color2 );

#endif /* __FW_SCR_H__ */

