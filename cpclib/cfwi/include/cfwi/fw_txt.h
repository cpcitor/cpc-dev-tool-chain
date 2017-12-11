#ifndef  __FW_TXT_H__
#define __FW_TXT_H__

#include <stdint.h>
#include "cfwi_byte_shuffling.h"

/** 26: TXT INITIALISE #BB4E
    Initialise the Text VDU.
    Action:
    Full initialization of the Text VDU (as used during EMS). All Text VDU variables and
    indirections are initialized, the previous VDU state is lost.
    Entry conditions:
    No conditions:
    Exit conditions:
    AF,BC,DE and HL corrupt. All other registers preserved.
    Notes:
    The Text VDU indirections (TXT DRAW CURSOR, TXT UNDRAW CURSOR, TXT
    WRITE CHAR, TXT UNWRITE and TXT OUT ACTION) are set to their default
    routines.
    The control code table is set up to perform the default control code actions.
    The user defined character table is set to empty.
    Stream 0 is selected.
    All streams are set to their default states:
    The text paper (background) is set to ink 0. The text pen (foreground) is set to ink 1.
    The text window is set to the entire screen. The text cursor is enabled but turned off.
    The character write mode is set to opaque.
    The VDU is enabled. The graphics character write mode is turn off. The cursor is
    moved to the top left corner of the window.
    The default character set and the default setting for the control code table are
    described in Appendices VI and VII.
    Related entries:
    SCR INITIALISE
    TXT RESET



    CFWI_TEST_FLAGS: TESTED_APP_PASS
*/
void fw_txt_initialise(void);

/** 27: TXT RESET
    #BB51
    Reset the Text VDU.
    Action:
    Reinitialize the Text VDU indirections and the control code tables. Does not affect
    any other aspect of the Text VDU.
    Entry conditions:
    No conditions.
    Exit conditions:
    AF,BC,DE and HL corrupt. All other registers preserved.
    Notes:
    The Text VDU indirections TXT DRAW CURSOR, TXT UNDRAW CURSOR, TXT
    WRITE CHAR, TXT UNWRITE and TXT OUT ACTION are set to their default
    routines.
    The control code table is set up to perform the default control code actions (see
    Appendix VII).
    Related entries:
    TXT INITIALISE



    CFWI_TEST_FLAGS: TESTED_APP_PASS
*/
void fw_txt_reset(void);

/** 28: TXT VDU ENABLE #BB54
    Allow characters to be placed on the screen.
    Action:
    Permit characters to be printed when requested (by calling TXT OUTPUT or TXT WR
    CHAR). Enabling applies to the currently selected stream. The cursor blob is also
    enabled (by calling TXT CUR ENABLE).
    Entry conditions:
    No conditions.
    Exit conditions:
    AF corrupt. All other registers preserved.
    Notes:
    The control code buffer used by TXT OUTPUT is emptied, any incomplete control
    code sequence will be lost.
    Related entries:
    TXT ASK STATE
    TXT CUR ENABLE
    TXT OUTPUT
    TXT VDU DISABLE
    TXT WR CHAR
*/
void fw_txt_vdu_enable(void);

/** 29: TXT VDU DISABLE #BB57
    Prevent character being placed on the screen.
    Action:
    Prevents characters being printed on the screen (when TXT OUTPUT or TXT WR
    CHAR is called). Applies to the currently selected stream. The cursor blob is also
    disabled (by calling TXT CUR DISABLE).
    Entry conditions:
    No conditions.
    Exit conditions:
    AF corrupt. All other registers preserved.
    Notes:
    The control code buffer used by TXT OUTPUT is emptied, any incomplete control
    code sequence will be lost.
    In V1.1 firmware control codes are still obeyed by TXT OUTPUT. In V1.1 firmware
    only those control codes which are marked in the control code table will be obeyed;
    other control codes will be ignored (see section 4.7).
    Related entries:
    TXT ASK STATE
    TXT CUR ENABLE
    TXT OUTPUT
    TXT VDU DISABLE
    TXT WR CHAR
*/
void fw_txt_vdu_disable(void);

/** 30: TXT OUTPUT
    #BB5A
    Output a character or control code to the Text VDU.
    Action:
    Output characters to the screen and obey control codes (characters #00..#1F). Works
    on the currently selected stream.
    Entry conditions:
    A contains the character to send.
    Exit conditions:
    All registers and flags preserved.
    Notes:
    This routine calls the TXT OUT ACTION indirection to do the work of printing the
    character or obeying the control code described below.
    Control codes may take up to 9 parameters. These are the characters sent following
    the initial control code. The characters sent are stored in the control code buffer until
    sufficient have been received to make up all the parameters. The control code buffer
    is only long enough to accept 9 parameter characters.
    There is only one control code buffer for all streams. It is therefore possible to get
    unpredictable results if the output stream is changed midway through sending a
    control code sequence.
    If the VDU is disabled then no characters will be printed on the screen. In V1.0
    firmware all control codes will still be obeyed but in V1.1 firmware only those codes
    marked in the control code table as to be obeyed when the VDU is disabled will be
    obeyed (see section 4.7).
    If the graphic character write mode is enabled then all characters and control codes
    are printed using the Graphics VDU routine, GRA WR CHAR, and are not obeyed.
    Characters are written in the same way that TXT WR CHAR writes characters.
    Related entries:
    GRA WR CHAR
    TXT OUT ACTION
    TXT SET GRAPHIC
    TXT VDU DISABLE
    TXT VDU ENABLE
    TXT WR CHAR



    CFWI_TEST_FLAGS: TESTED_APP_PASS
*/
void fw_txt_output(unsigned char c) __z88dk_fastcall;

/** 31: TXT WR CHAR #BB5D
    Write a character to the screen.
    Action:
    Print a character on the screen at the cursor position of the currently selected stream.
    Control codes (characters #00..#1F) are printed and not obeyed.
    Entry conditions:
    A contains the character to print.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    If the VDU is disabled then no character will be printed.
    Before printing the character the cursor position is forced to lie within the text
    window (see TXT VALIDATE). After printing the character the cursor is moved right
    one character.
    To put the character on the screen this routine calls the TXT WRITE CHAR
    indirection.
    Related entries:
    GRA WR CHAR
    TXT OUTPUT
    TXT RD CHAR
    TXT WRITE CHAR



    CFWI_TEST_FLAGS: TESTED_APP_PASS
*/
void fw_txt_wr_char(unsigned char c) __z88dk_fastcall;

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    #### CFWI-specific information: ####

    since C cannot handle carry flag, value is returned like this:

    uint16_t returned_value = fw_txt_rd_char();
    if (UINT_AND_BYTE_1(returned_value))
    {
    // a character was read
    unsigned char c = UINT_SELECT_BYTE_0(returned_value);
    }
    else
    {
    // no character was read
    }


    32: TXT RD CHAR
    #BB60
    Read a character from the screen.
    Action:
    Read a character from the screen at the cursor position of the currently selected
    stream.
    Entry conditions:
    No conditions.
    Exit conditions:
    If a recognisable character was found:
    Carry true.
    A contains the character read.
    If no recognisable character was found:
    Carry false.
    A contains zero.
    Always:
    Other flags corrupt.
    All other registers preserved.
    Notes:
    In V1.1 firmware the cursor position is forced legal (inside the window) before the
    character is read. This may cause the screen to roll. The same is not true in V1.0
    firmware where the cursor position is not forced legal and steps must be taken to
    avoid reading characters from outside the window.
    The read is performed by comparing the matrix found on the screen with the matrices
    used to generate characters. As a result changing a character matrix, changing the pen
    or paper inks, or changing the screen (e.g. drawing a line through a character) may
    make the character unreadable.
    To actually read the character from the screen the TXT UNWRITE indirection is
    called.
    Special precautions are taken against generating inverse space (character #8F).
    Initially the character is read assuming that the background to the character was
    written in the current paper ink and treating any other ink as foreground. If this fails
    to generate a recognisable character or it generates inverse space then another try is
    made by assuming that the foreground to the character was written in the current pen
    ink and treating any other ink as background.
    The characters are scanned starting with #00 and finishing with #FF.
    Related entries:
    TXT UNWRITE
    TXT WR CHAR
*/
uint16_t fw_txt_rd_char(void);

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    33: TXT SET GRAPHIC #BB63
    Turn on or off the Graphics VDU write character option.
    Action:
    Enable or disable graphic character writing on the currently selected stream.
    Entry conditions:
    If graphic writing is to be turned on:
    A must be non-zero.
    If the graphic writing is to be turned off:
    A must contain zero.
    Exit conditions:
    AF corrupt.
    All other registers preserved.
    Notes:
    When graphic character writing is enabled then all characters sent to TXT OUTPUT
    are printed using the Graphics VDU (see GRA WR CHAR) rather then the Text VDU
    (see TXT WR CHAR). Also all control codes are printed rather than obeyed.
    Characters sent to TXT WR CHAR will be printed as normal.
    Character printing is not prevented by disabling the Text VDU (with TXT VDU
    DISABLE) if graphic character writing is enabled.
    Related entries:
    GRA WR CHAR
    TXT OUTPUT
*/
void fw_txt_set_graphic(bool enable) __z88dk_fastcall;

/** FIXME UNUSED Values are zero-based:

 * column values are between 0 and 19,39 or 79 included, depending on mode.
 * line values are between 0 and 24, included.

*/
typedef struct fw_txt_window_t
{
	uint8_t top;
	uint8_t left;
	uint8_t bottom;
	uint8_t right;
} fw_txt_window_t;

/** #### CFWI-specific information: ####

    Firmware documentation says number are signed, which seems to make little sense.
    We declare them unsigned because SDCC generates simpler code.


    34: TXT WIN ENABLE #BB66
    Set the size of the current text window.
    Action:
    Set the boundaries of the window on the currently selected stream. The edges are the
    first and last character columns inside the window and the first and last character
    rows inside the window.
    Entry conditions:
    H contains the physical column of one edge.
    D contains the physical column of the other edge.
    L contains the physical row of one edge.
    E contains the physical row of the other edge.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    The edge positions are given in physical screen coordinates. i.e. Row 0, column 0 is
    the top left corner of the screen and the coordinates are signed numbers.
    The window is truncated, if necessary, so that it fits on the screen.
    The left column of the window is taken to be the smaller of H and D. The top row of
    the window is taken to be the smaller of L and E.
    The cursor is moved to the top left corner of the window.
    The window is not cleared.
    If the window covers the whole screen then when the window is rolled the hardware
    roll routine (see SCR HW ROLL) will be used. If the window covers less than the
    whole screen the software roll routine (see SCR SW ROLL) will be used.
    The default text window covers the whole screen and is set up when TXT
    INITIALISE or SCR SET MODE is called.
    Related entries:
    TXT GET WINDOW
    TXT VALIDATE



    CFWI_TEST_FLAGS: TESTED_APP_PASS
*/
void fw_txt_win_enable(uint8_t left, uint8_t right, uint8_t top, uint8_t bottom);

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    #### CFWI-specific information: ####

    since SDCC cannot return struct from a function, value is returned like this:

    uint32_t returned_value = fw_txt_get_window();
    uint8_t left = UINT_SELECT_BYTE_1(returned_value);
    uint8_t right = UINT_SELECT_BYTE_3(returned_value);
    uint8_t top = UINT_SELECT_BYTE_0(returned_value);
    uint8_t bottom = UINT_SELECT_BYTE_2(returned_value);

    35: TXT GET WINDOW #BB69
    Get the size of the current window.
    Action:
    Get the boundaries of the window on the currently selected stream and whether it
    covers the whole screen.
    Entry conditions:
    No conditions.
    Exit conditions:
    If the window covers the whole screen:
    Carry false.
    If the window covers less than the whole screen:
    Carry true.
    Always:
    H contains the leftmost column in the window.
    D contains the rightmost column in the window.
    L contains the topmost row in the window.
    E contains the bottommost row in the window.
    A corrupt.
    All other registers preserved.
    Notes:
    The boundaries of the window are given in physical coordinates. i.e. Row 0, column 0
    is the top left corner of the screen.
    The boundaries returned by this routine may not be the same as those set when TXT
    WIN ENABLE was called because the window is truncated to fit the screen.
    Related entries:
    TXT VALIDATE
    TXT WIN ENABLE
*/
uint32_t fw_txt_get_window(void);

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    36: TXT CLEAR WINDOW #BB6C
    Clear current window.
    Action:
    Clear the text window of the currently selected stream to the paper ink of the
    currently selected stream.
    Entry conditions:
    No conditions.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    The cursor is moved to the top left corner of the window.
    Related entries:
    GRA CLEAR WINDOW
    SCR CLEAR
    TXT SET PAPER
    TXT WIN ENABLE
*/
void fw_txt_clear_window(void);

void fw_txt_cur_enable(void);
void fw_txt_cur_disable(void);
void fw_txt_cur_on(void);
void fw_txt_cur_off(void);
void fw_txt_place_cursor(void);
void fw_txt_remove_cursor(void);
void fw_txt_draw_cursor(void);
void fw_txt_undraw_cursor(void);

void fw_txt_set_column(int8_t column);
void fw_txt_set_row(int8_t row);
void fw_txt_set_cursor(int8_t row, int8_t column);


void fw_txt_set_pen(uint8_t p);
void fw_txt_set_paper(uint8_t p);


void fw_txt_set_m_table(void *buffer, bool disable, uint8_t lowest_affected_character);

#endif /* __FW_TXT_H__ */
