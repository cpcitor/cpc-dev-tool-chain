#ifndef  __FW_SCR_H__
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
void fw_scr_initialise(void)  __preserves_regs(iyh, iyl);

void fw_scr_set_mode(unsigned char x);

void fw_scr_reset(void);
void fw_scr_clear(void);
void fw_scr_mode_clear(void);
void fw_scr_set_ink( uint8_t pen, uint8_t color1, uint8_t color2 );
void fw_scr_set_border( uint8_t color1, uint8_t color2 );

#endif /* __FW_SCR_H__ */
