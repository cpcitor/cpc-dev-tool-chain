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

/** Used for fw_txt_win_enable() and to decode fw_txt_get_window().

 * column values are between 0 and 19,39 or 79 included, depending on mode.
 * line values are between 0 and 24, included.

*/
typedef union fw_txt_window_t
{
	struct
	{
		uint8_t top;
		uint8_t left;
		uint8_t bottom;
		uint8_t right;
	};
	uint32_t as_uint32_t;
} fw_txt_window_t;

/** #### CFWI-specific information: ####

    Firmware documentation says number are signed, which seems to make little sense.
    We declare them unsigned because SDCC generates simpler code.

    Two variants are provided: one with wrapper, one with integer.
    You can use either, as you see fit.



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

    Use the fastcall variant like this:

    fw_txt_window_t window_def = { 2, 2, 22, 37 }; // example values
    fw_txt_win_enable__fastcall(window_def.as_uint32_t);
 */
    void fw_txt_win_enable__fastcall(uint32_t fw_txt_window_t_asint) __z88dk_fastcall;

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    #### CFWI-specific information: ####

    since SDCC cannot return struct from a function, value is returned like this:

    fw_txt_window_t window_spec;
    window_spec.as_uint32_t = fw_txt_get_window();
    printf("%d\n", window_spec.right);

    You can also decode values directly from the uint32_t be be aware
    that it leads to inefficient code.

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

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    37: TXT SET COLUMN #BB6F
    Set cursor horizontal position.
    Action:
    Move the current position of the currently selected stream to a new column. The
    cursor blob will be removed from the current position and redrawn at the new position
    (if the cursor is enabled and turned on).
    Entry conditions:
    A contains the required logical column for the cursor.
    Exit conditions:
    AF and HL corrupt.
    All other registers preserved.
    Notes:
    The required column is given in logical coordinates. i.e. Column 1 is the leftmost
    column of the window.
    The cursor may be moved outside the window. However, it will be forced to lie inside
    the window before any character is written by the Text VDU (see TXT VALIDATE) or
    the cursor blob is drawn.
    Related entries:
    TXT GET CURSOR
    TXT SET CURSOR
    TXT SET ROW
*/
void fw_txt_set_column(int8_t column) __z88dk_fastcall;

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    38: TXT SET ROW
    #BB72
    Set cursor vertical position.
    Action:
    Move the current position of the currently selected stream to a new row. The cursor
    blob will be removed from the current position and redrawn at the new position (if the
    cursor is enabled and turned on).
    Entry conditions:
    A contains the required logical row for the cursor.
    Exit conditions:
    AF and HL corrupt.
    All other registers preserved.
    Notes:
    The required row is given in logical coordinates. i.e. Row 1 is the topmost row of the
    window.
    The cursor may be moved outside the window. However, it will be forced to lie inside
    the window before any characters is written by the Text VDU (see TXT VALIDATE)
    or the cursor blob is drawn.
    Related entries:
    TXT GET CURSOR
    TXT SET COLUMN
    TXT SET CURSOR
*/
void fw_txt_set_row(int8_t row) __z88dk_fastcall;

/** #### CFWI-specific information: ####

    You can use the fw_txt_set_cursor__fastcall() variant for slightly
    shorter generated ASM code (no need for wrapper in that case).
    In most case don't bother just write short C source code.

    39: TXT SET CURSOR #BB75
    Set cursor position.
    Action:
    Move the current position of the currently selected stream to a new row and column.
    The cursor blob will be removed from the current position and redrawn at the new
    position (if the cursor is enabled and turned on).
    Entry conditions:
    H contains the required logical column.
    L contains the required logical row.
    Exit conditions:
    AF and HL corrupt.
    All other registers preserved.
    Notes:
    The required row is given in logical coordinates. i.e. Row 1, column 1 is the top left
    corner of the window.
    The cursor may be moved outside the window. However, it will be forced to lie inside
    the window before any characters is written by the Text VDU (see TXT VALIDATE)
    or the cursor blob is drawn.
    Related entries:
    TXT GET CURSOR
    TXT SET COLUMN
    TXT SET ROW



    CFWI_TEST_FLAGS: TESTED_APP_PASS
*/
void fw_txt_set_cursor(int8_t row, int8_t column);
void fw_txt_set_cursor__fastcall(int16_t colum8h_row8l) __z88dk_fastcall;


/** Can be used to decode output of fw_txt_get_cursor(). */
typedef union fw_txt_cursor_pos_t
{
	struct
	{
		uint8_t row;
		uint8_t column;
		uint8_t roll_count;
	};
	uint32_t as_uint32_t;
} fw_txt_cursor_pos_t;


/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    #### CFWI-specific information: ####

    You can use it like this:

    fw_txt_cursor_pos_t cursor_pos;
    cursor_pos.as_uint32_t = fw_txt_get_window();
    printf("%d\n", cursor_pos.roll_count);

    You can also decode values directly from the uint32_t be be aware
    that it leads to inefficient code.

    uint32_t returned_value = fw_txt_get_cursor();
    uint8_t row = UINT_SELECT_BYTE_0(returned_value);
    uint8_t column = UINT_SELECT_BYTE_1(returned_value);
    uint8_t roll_count = UINT_SELECT_BYTE_2(returned_value);


    40: TXT GET CURSOR #BB78
    Ask current cursor position.
    Action:
    Get the current location of the cursor and a count of the number of times the window
    of the currently selected stream has rolled.
    Entry conditions:
    No conditions.
    Exit conditions:
    H contains the logical cursor column.
    L contains the logical cursor row.
    A contains the current roll count.
    Flags corrupt.
    All other registers preserved.
    Notes:
    The cursor position is given in logical coordinates. i.e. Row 1, column 1 is the top left
    corner of the window.
    The roll count passed out has no absolute meaning. It is decremented when the
    window is rolled up and is incremented when the window is rolled down. It may be
    used to determine whether the window has rolled by comparing it with a previous
    value.
    The position reported may not be inside the window and is, therefore, not necessarily
    the position at which the next character will be printed. Use TXT VALIDATE to check
    this.
    Related entries:
    TXT SET COLUMN
    TXT SET CURSOR
    TXT SET ROW
    TXT VALIDATE
*/
uint32_t fw_txt_get_cursor();

/** 41: TXT CUR ENABLE #BB7B
    Allow cursor display - user.
    Action:
    Allow the cursor blob for the currently selected stream to be placed on the screen. The
    cursor blob will be placed on the screen immediately unless the cursor is turned off
    (see TXT CUR OFF).
    Entry conditions:
    No conditions.
    Exit conditions:
    AF corrupt.
    All other registers preserved.
    Notes:
    Cursor enabling and disabling is intended for use by the user. It is also used when the
    VDU is disabled (TXT VDU ENABLE and TXT VDU DISABLE).
    Related entries:
    TXT ASK STATE
    TXT CUR DISABLE
    TXT CUR ON
    TXT DRAW CURSOR
    TXT UNDRAW CURSOR
*/
void fw_txt_cur_enable(void);

/** 42: TXT CUR DISABLE #BB7E
    Disallow cursor display - user.
    Action:
    Prevent the cursor blob for the currently selected stream from being placed on the
    screen. The cursor blob will be removed from the screen immediately if it is currently
    there.
    Entry conditions:
    No conditions.
    Exit conditions:
    AF corrupt.
    All other registers preserved.
    Notes:
    Cursor enabling and disabling is intended for use by the user. It is also used when the
    VDU is disabled (TXT VDU ENABLE and TXT VDU DISABLE).
    Related entries:
    TXT ASK STATE
    TXT CUR ENABLE
    TXT CUR OFF
    TXT DRAW CURSOR
    TXT UNDRAW CURSOR
*/
void fw_txt_cur_disable(void);

/** 43: TXT CUR ON
    #BB81
    Allow cursor display - system.
    Action:
    Allow the cursor blob for the currently selected stream to be placed on the screen. The
    cursor blob will be placed on the screen immediately unless the cursor in disabled
    (see TXT CUR DISABLE).
    Entry conditions:
    No conditions.
    Exit conditions:
    All registers and flags preserved.
    Notes:
    Turning the cursor on and off is intended for use by system ROMs.
    Related entries:
    TXT ASK STATE
    TXT CUR ENABLE
    TXT CUR OFF
    TXT DRAW CURSOR
    TXT UNDRAW CURSOR
*/
void fw_txt_cur_on(void);

/** 44: TXT CUR OFF
    #BB84
    Disallow cursor display - system.
    Action:
    Prevent the cursor blob for the currently selected stream from being placed on the
    screen. The cursor blob will be removed from the screen immediately if it is currently
    there.
    Entry conditions:
    No conditions.
    Exit conditions:
    All registers and flags preserved.
    Notes:
    Turning the cursor on and off is intended for use by system ROMs.
    Related entries:
    TXT ASK STATE
    TXT CUR DISABLE
    TXT CUR ON
    TXT DRAW CURSOR
    TXT UNDRAW CURSOR
*/
void fw_txt_cur_off(void);


/** Can be used to decode output of fw_txt_validate(). */
typedef union fw_txt_cursor_validation_t
{
	struct
	{
		uint8_t row;
		uint8_t column;
		/** 0xff if would scroll, else 0. */
		uint8_t would_scroll;
		/** 0xff if would scroll up, 0 if would scroll down. */
		uint8_t scroll_direction;
	};
	uint32_t as_uint32_t;
} fw_txt_cursor_validation_t;

enum fw_txt_validate_scroll_direction
{
	fw_txt_validate_scroll_direction_up = fw_byte_all,
	fw_txt_validate_scroll_direction_down = fw_byte_nothing
};

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    #### CFWI-specific information: ####

    You can use it like this:

    fw_txt_cursor_validation_t cursor_validation;
    cursor_validation.as_uint32_t = fw_txt_get_window();
    printf("%d,%d ", cursor_validation.row, cursor_validation.column);
    if (cursor_validation.would_scroll)
    {
    // would scroll
    if (cursor_validation.scroll_direction)
    {
    // would scroll up
    }
    else
    {
    // would scroll down
    }
    }
    else
    {
    // would not scroll
    }

    You can also decode values directly from the uint32_t be be aware
    that it leads to inefficient code.

    uint32_t returned_value = fw_txt_validate();
    uint8_t destination_row = UINT_SELECT_BYTE_0(returned_value);
    uint8_t destination_col = UINT_SELECT_BYTE_1(returned_value);
    if (UINT_SELECT_BYTE_2(returned_value))
    {
    // would scroll
    fw_txt_validate_scroll_direction scroll_direction = UINT_SELECT_BYTE_3(returned_value);
    }
    else
    {
    // would not scroll
    }


    45: TXT VALIDATE
    #BB87
    Check if a cursor position is within the window.
    Action:
    Check a screen position to see if it lies within the current window. If it does not then
    determine the position where a character would be printed after applying the rules for
    forcing the screen position inside the window.
    Entry conditions:
    H contains the logical column of the position to check.
    L contains the logical row of the position to check.
    Exit conditions:
    If printing at the position would not cause the window to roll:
    Carry true.
    B corrupt.
    If printing at the position would cause the window to roll up:
    Carry false.
    B contains #FF.
    If printing at the position would cause the window to roll down:
    Carry false.
    B contains #00.
    Always:
    H contains the logical column at which a character would be printed.
    L contains the logical row at which a character would be printed.
    A and other flags corrupt.
    All other registers preserved.
    Notes:
    The position on the screen are given in logical coordinates. i.e. Row 1, column 1 is
    the top left corner of the window.
    Before writing a character or putting the cursor blob on the screen the Text VDU
    validates the current position, performs any required roll then writes at the
    appropriate position.
    The algorithm to work out the position to print at, from the position to check, is as
    follows:
    1/ If the position is right of the right edge of the window it is moved to the left edge
    of the window on the next line.
    2/ If the position is left of the left edge of the window it is moved to the right edge of
    the window on the previous line.
    3/ If the position is now above the top edge of the window then it is moved to the top
    edge of the window and the window need rolling downwards.
    4/ If the position is now below the bottom edge of the window it is moved to the
    bottom edge of the window and the window needs rolling upwards.
    Related entries:
    SCR HW ROLL
    SCR SW ROLL
    TXT GET CURSOR
*/
uint32_t fw_txt_validate(void);

/** 46: TXT PLACE CURSOR #BB8A
    Put a cursor blob on the screen.
    Action:
    Put a cursor blob on the screen at the cursor position for the currently selected stream.
    Entry conditions:
    No conditions.
    Exit conditions:
    AF corrupt.
    All other registers preserved.
    Notes:
    TXT PLACE CURSOR is provided to allow the user to run multiple cursors in a
    window. The indirection TXT DRAW CURSOR should be called for merely placing
    the normal cursor blob on the screen. Higher level routines, such as TXT OUTPUT
    and TXT SET CURSOR, automatically remove and place the normal cursor when
    appropriate, the user must deal with any other cursors.
    It is not safe to call TXT PLACE CURSOR twice at a particular screen position
    without calling TXT REMOVE CURSOR in between because this may leave a
    spurious cursor blob on the screen when the cursor position is moved.
    The cursor position is forced to be inside the window before the cursor blob is drawn.
    The cursor blob is an inverse patch formed by exclusive-oring the contents of the
    screen at the cursor position with the exclusive-or of the current pen and paper inks.
    Related entries:
    TXT DRAW CURSOR
    TXT REMOVE CURSOR
*/
void fw_txt_place_cursor(void);

/** 47: TXT REMOVE CURSOR #BB8D
    Take a cursor blob off the screen.
    Action:
    Take the cursor blob off the screen at the cursor position of the currently selected
    stream.
    Entry conditions:
    No conditions.
    Exit conditions:
    AF corrupt.
    All other registers preserved.
    Notes:
    TXT REMOVE CURSOR is provided to allow the user to run multiple cursors in a
    window. The indirection TXT UNDRAW CURSOR should be called for merely
    removing the normal from the screen. Higher level routines, such as TXT OUTPUT
    and TXT SET CURSOR, automatically remove and place the normal cursor when
    appropriate, the user must deal with any other cursors.
    TXT REMOVE CURSOR should only be used to remove a cursor placed on the
    screen by calling TXT PLACE CURSOR. The cursor should be removed when the
    cursor position is to be changed (rolling the window implicitly changes the cursor
    position) or the screen is to read or written. Incorrect use of this routine may result in
    a spurious cursor blob being generated.
    The cursor position is forced to be inside the window before the cursor blob is
    removed (this should not matter as TXT PLACE CURSOR has already done this).
    The cursor blob is an inverse patch formed by exclusive-oring the contents of the
    screen at the cursor position with the exclusive-or of the current pen and paper inks.
    Related entries:
    TXT PLACE CURSOR
    TXT UNDRAW CURSOR
*/
void fw_txt_remove_cursor(void);

/** 48: TXT SET PEN
    #BB90
    Set ink for writing characters.
    Action:
    Set the pen ink for the currently selected stream. This is the ink that is used for
    writing characters (the foreground ink).
    Entry conditions:
    A contains ink to use.
    Exit conditions:
    AF and HL corrupt.
    All other registers preserved.
    Notes:
    The ink is masked to bring it within the range of legal inks for the current screen
    mode. That is with #0F in mode 0, #03 in mode 1 and #01 in mode 2.
    The cursor blob will be redrawn using the new ink (if enabled).
    Related entries:
    GRA SET PEN
    SCR SET INK
    TXT GET PEN
    TXT SET PAPER
*/
void fw_txt_set_pen(uint8_t p) __z88dk_fastcall;

/** 49: TXT GET PEN
    #BB93
    Get ink for writing characters.
    Action:
    Ask what the pen ink is set to for the currently selected stream. This is the ink used
    for writing characters (foreground ink).
    Entry conditions:
    No conditions.
    Exit conditions:
    A contains the ink.
    Flags corrupt.
    All other registers preserved.
    Notes:
    This routine has no other effects.
    Related entries:
    GRA GET PEN
    SCR GET INK
    TXT GET PAPER
    TXT SET PEN
*/
uint8_t fw_txt_get_pen(void);

/** 50: TXT SET PAPER #BB96
    Set ink for writing text background.
    Action:
    Set the text paper ink for the currently selected stream. This is the ink used for
    writing the background to characters and for clearing the text window.
    Entry conditions:
    A contains the ink to use.
    Exit conditions:
    AF and HL corrupt.
    All other registers preserved.
    Notes:
    The ink is masked to bring it within the range of legal inks for the current screen
    mode. That is with #0F in mode 0, #03 in mode 1 and #01 in mode 2.
    The cursor blob will be redrawn using the new ink (if enabled).
    This ink will be used when clearing areas of the text window (by TXT CLEAR
    WINDOW and certain control codes).
    This routine does not clear the text window.
    Related entries:
    GRA GET PAPER
    SCR SET INK
    TXT GET PAPER
    TXT SET PEN
*/
void fw_txt_set_paper(uint8_t p) __z88dk_fastcall;

/** 51: TXT GET PAPER #BB99
    Get ink for writing background.
    Action:
    Ask what the paper ink is set to for the currently selected stream. The ink used for
    writing the background to characters and for clearing the text window.
    Entry conditions:
    No conditions.
    Exit conditions:
    A contains the ink.
    Flags corrupt.
    All other registers preserved.
    Notes:
    This routine has no other effects.
    Related entries:
    GRA GET PAPER
    SCR GET INK
    TXT GET PEN
    TXT SET PAPER
*/
uint8_t fw_txt_get_paper(void);

/** 52: TXT INVERSE #BB9C
    Swap current pen and paper inks over.
    Action:
    Exchange the text pen and paper (foreground and background) inks for the currently
    selected stream.
    Entry conditions:
    No conditions.
    Exit conditions:
    AF and HL corrupt.
    All other registers preserved.
    Notes:
    In V1.1 firmware the cursor blob is removed and replaced and so the current position
    is forced legal (inside the window) which may cause the window to roll. In V1.0
    firmware the cursor blob is not redrawn and so it should be on the screen when this
    routine is called.
    Related entries:
    TXT SET PAPER
    TXT SET PEN
*/
void fw_txt_inverse(void);

/** 53: TXT SET BACK
    #BB9F
    Allow or disallow background being written.
    Action:
    Set character write mode to opaque or transparent for the currently selected stream.
    Opaque mode writes background with the character. Transparent mode writes the
    character on top of the current contents of the screen.
    Entry conditions:
    If the background is to be written (opaque mode):
    A must be zero.
    If background is not to be written (transparent mode):
    A must be non-zero.
    Exit conditions:
    AF and HL corrupt.
    All other registers preserved.
    Notes:
    Writing in transparent mode is intended for annotating diagrams and similar
    applications. It can have unfortunate effects if it is used generally because overwriting
    a character will not remove the character underneath thus creating an
    incomprehensible jumble on the screen.
    Setting the character write mode does not affect the Graphics VDU. In V1.1 firmware
    the routine GRA SET BACK sets the equivalent graphics background write mode.
    Related entries:
    GRA SET BACK
    TXT GET BACK
    TXT WR CHAR
    TXT WRITE CHAR
*/
void fw_txt_set_back(uint8_t is_transparent) __z88dk_fastcall;

/** 54: TXT GET BACK #BBA2
    Ask if background is being written.
    Action:
    Get the character write mode for the currently selected stream.
    Entry conditions:
    No conditions.
    Exit conditions:
    If background is to be written (opaque mode):
    A contains zero.
    If background is not to be written (transparent mode):
    A contains non-zero.
    Always:
    DE, HL and flags corrupt.
    All registers preserved.
    Notes:
    This only applies to the Text VDU, the Graphics VDU always writes opaque.
    Related entries:
    TXT SET BACK
*/
uint8_t fw_txt_get_back(void);

typedef uint8_t fw_txt_character_matrix_t[8];

/** Can be used to decode output of fw_txt_validate(). */
typedef union fw_txt_p_character_matrix_with_rom_indication_t
{
	struct
	{
		fw_txt_character_matrix_t *p_matrix;
		enum fw_byte_all_or_nothing is_user_defined;
	};
	uint32_t as_uint32_t;
} fw_txt_p_character_matrix_with_rom_indication_t;

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    #### CFWI-specific information: ####

    Since C cannot handle carry flag, the information is returned like this:

    uint8_t character_index = (uint8_t)'C';
    fw_txt_p_character_matrix_with_rom_indication_t pcmwri;
    pcmwri.as_uint32_t = fw_txt_get_matrix(character_index);
    fw_txt_character_matrix_t *matrix = pcmwri.p_matrix;
    if (pcmwri.is_user_defined)
    {
    // is in RAM
    }
    else
    {
    // is in ROM
    }


    55: TXT GET MATRIX #BBA5
    Get the address of a character matrix.
    Action:
    Calculate a pointer to the matrix for a character and determine if it is a user defined
    matrix.
    Entry conditions:
    A contains the character whose matrix is to be found.
    Exit conditions:
    If the matrix in the user defined matrix table:
    Carry true.
    If the matrix is in the lower ROM:
    Carry false.
    Always:
    HL contains the address of the matrix.
    A and other flags corrupt.
    All other registers preserved.
    Notes:
    The matrix may be in RAM or in ROM. The Text VDU assumes that the appropriate
    ROMs are enabled or disabled when it calls this routine to get the matrix for a
    character. (The lower ROM is on, the upper ROM is normally off).
    The matrix is stored as an 8 byte bit significant vector. The first byte describes the top
    line of the character and the last byte the bottom line. Bit 7 of a byte refers to the
    leftmost pixel of a line and bit 0 to the rightmost pixel. If a bit is set in the matrix
    then the pixel should be written in the pen ink. If the bit is not set then the pixel
    should either be written in the paper ink or left alone (depending on the character
    write mode).
    Related entries:
    TXT SET MATRIX
*/
fw_txt_p_character_matrix_with_rom_indication_t *fw_txt_get_matrix(uint8_t character_number) __z88dk_fastcall;

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    #### CFWI-specific information: ####

    Since C cannot handle carry flag, function returns 0xff if character could be defined, else 0.



    56: TXT SET MATRIX #BBA8
    Set a character matrix.
    Action:
    Set the matrix for a user defined character. If the character is not user defined then no
    action is taken.
    Entry conditions:
    A contains the character whose matrix is to be set.
    HL contains the address of the matrix to set.
    Exit conditions:
    If the character is user definable:
    Carry true.
    If the character is not user definable:
    Carry false.
    Always:
    A, BC, DE, HL and other flags corrupt.
    All other registers preserved.
    Notes:
    The matrix is stored as an 8 byte bit significant vector. The first byte describes the top
    line of the character and the last byte the bottom line. Bit 7 of a byte refers to the
    leftmost pixel of a line and bit 0 to the rightmost pixel. If a bit is set in the matrix
    then the pixel should be written in the pen ink. If a bit is not set then the pixel should
    either be written in the paper ink or left alone (depending whether the character write
    mode is opaque or transparent currently).
    The matrix is copied from the area given into the character matrix table without using
    RAM LAMs thus the matrices can be set from ROM providing it is enabled. (Note
    however that the jumpblock disables the upper ROM).
    Altering a character matrix changes the matrix for all streams. It does not alter any
    character on the screen; it changes what will be placed on the screen the next time the
    character is written.
    Related entries:
    TXT GET MATRIX
    TXT SET M TABLE
*/
enum fw_byte_all_or_nothing fw_txt_set_matrix(uint8_t character, fw_txt_character_matrix_t *matrix);

void fw_txt_draw_cursor(void);
void fw_txt_undraw_cursor(void);


/** Can be used to decode output of fw_txt_set_m_table(). */
typedef union fw_txt_p_character_matrix_with_size_and_valid_t
{
	struct
	{
		fw_txt_character_matrix_t *p_matrix;
		uint8_t lowest_character_defined;
		enum fw_byte_all_or_nothing is_valid;
	};
	uint32_t as_uint32_t;
} fw_txt_p_character_matrix_with_size_and_valid_t;

/** #### CFWI-specific information: ####

    Since C cannot handle carry flag, the information is returned like this:

    fw_txt_character_matrix_t my_new_chars[16] = { ... }
    fw_txt_p_character_matrix_with_size_and_valid_t pcmwsav;
    pcmwsav.as_uint32_t = fw_txt_set_m_table(my_new_chars, 0, 256-16);
    if (pcmwsav.is_valid)
    {
    fw_txt_character_matrix_t *matrix = pcmwsav.p_matrix;
    printf("There were already some defined characters from %d at address 0x%04x.\n",
    pcmwsav.is_valid,  pcmwsav.p_matrix);
    }
    else
    {
    printf("There were no defined characters.\n",
    }



    57: TXT SET M TABLE #BBAB
    Set the user defined matrix table address.
    Action:
    Set the user defined matrix table and the number of characters in the table. The table
    is initialized with the current matrix settings.
    Entry conditions:
    DE contains the first character in the table.
    HL contains the address of the start of the new table.
    Exit conditions:
    If there was no user defined matrix table before:
    Carry false.
    A and HL corrupt.
    If there was a user defined matrix table before:
    Carry true.
    A contains the first character in the old table.
    HL contains the address of the old table.
    Always:
    BC, DE and other flags corrupt.
    All other registers preserved.
    Notes:
    If the first character specified is in the range 0..255 then the matrices for all
    characters between that character and character 255 are to be stored in the user
    defined table.
    If the first character specified is not in the range 0..255 then the user defined matrix
    table is deemed to contain no matrices (and the table address passed is ignored).
    The table must be (256 - first char) * 8 bytes long. The matrices are stored in the table
    in ascending order. The table is initialized with the current matrix settings, whether
    they were previously in RAM or in the ROM.
    The table should not be located in RAM underneath a ROM.
    It is permissible for the new and old matrix tables to overlap (thus allowing the table
    to be extended or contracted) providing that matrices in the new table occupy an
    address earlier to the address that they occupied in the old table.
    All streams share the matrix table so any changes to it will be reflected on all streams.
    Related entries:
    TXT GET M TABLE
    TXT SET MATRIX



    CFWI_TEST_FLAGS: TESTED_APP_PASS
*/
uint32_t fw_txt_set_m_table(fw_txt_character_matrix_t *buffer, bool disable, uint8_t lowest_affected_character);

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    #### CFWI-specific information: ####

    Since C cannot handle carry flag, the information is returned like this:

    fw_txt_p_character_matrix_with_size_and_valid_t pcmwsav;
    pcmwsav.as_uint32_t = fw_txt_get_m_table();
    if (pcmwsav.is_valid)
    {
    fw_txt_character_matrix_t *matrix = pcmwsav.p_matrix;
    printf("There are some defined characters from %d at address 0x%04x.\n",
    pcmwsav.is_valid,  pcmwsav.p_matrix);
    }
    else
    {
    printf("There are no defined characters.\n",
    }



    58: TXT GET M TABLE #BBAE
    Get user defined matrix table address.
    Action:
    Get the address of the current user defined matrix table and the first character in the
    table.
    Entry conditions:
    No conditions.
    Exit conditions:
    If there is no user defined matrix table:
    Carry false.
    A and HL corrupt.
    If there is a user defined matrix table:
    Carry true.
    A contains the first character in the table.
    HL contains the address of the start of the table.
    Always:
    Other flags corrupt.
    All other registers preserved.
    Notes:
    The matrices for characters between the first character and 255 are stored in the table
    in ascending order. Each matrix is 8 bytes long.
    Related entries:
    TXT GET MATRIX
    TXT SET M TABLE
*/
uint32_t fw_txt_get_m_table();

enum
{
	fw_txt_control_code_count = 32
};

struct fw_txt_control_code_entry_t
{
	uint8_t parameter_count;
	void *routine;
};

typedef struct fw_txt_control_code_entry_t fw_txt_control_code_table_t[fw_txt_control_code_count];

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    59: TXT GET CONTROLS #BBB1
    Fetch address of control code table.
    Action:
    Get the address of the control code table.
    Entry conditions:
    No conditions.
    Exit conditions:
    HL contains the address of the control code table.
    All other registers and flags preserved.
    Notes:
    All streams share one control code table so that any changes made to the table will
    affect all streams.
    The control code table has a 3 byte entry for each control code. The entries are stored
    in ascending order, so the entry for #00 is first and that for #1F is last. The first byte
    of each entry is the number of parameters the control code requires, the other two
    bytes are the address of the routine to call to process the control code when all its
    parameters have been received. The routine must be located in the central 32K of
    RAM and it must obey the following interface:
    Entry:
    A contains the last character added to the buffer.
    B contains the length of the buffer (including the control code).
    C contains the same as A.
    HL contains the address of the control code buffer (points at the control code).
    Exit:
    AF, BC, DE, HL corrupt.
    All other registers preserved.
    As the control buffer only has space to store 9 parameter characters the number of
    parameters required should be limited to 9 or fewer.
    The control code table is reinitialized to its default routines when TXT RESET is
    called.
    In V1.1 firmware the first byte of each entry also specifies whether the control codes
    is to be disabled when the VDU is disabled or whether it is always to be obeyed. Bit 7
    of the byte is set if the code is to be disabled.
    Related entries:
    TXT OUTPUT
*/
fw_txt_control_code_table_t *fw_txt_get_controls();


#endif /* __FW_TXT_H__ */
