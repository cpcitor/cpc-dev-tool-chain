#ifndef  __FW_GRA_H__
#define __FW_GRA_H__

#include <stdint.h>

typedef union fw_gra_x_y_coordinates_t
{
	struct
	{
		int16_t y;
		int16_t x;
	};
	uint32_t as_uint32_t;
} fw_gra_x_y_coordinates_t;

/** 62: GRA INITIALISE
    #BBBA
    Initialize the Graphics VDU.
    Action:
    The Graphics VDU is fully initialized (as during EMS). All Graphic VDU variables
    and indirections are set to their default values.
    Entry conditions:
    No conditions.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    The full operation is:
    Set the Graphics VDU indirections (GRA PLOT, GRA TEST and GRA LINE) to
    their default routines.
    Set the graphics paper to ink 0.
    Set the graphics pen to ink 1.
    Set the user origin to the bottom left corner of the screen.
    Move the current position to the user origin.
    Set the graphics window to cover the whole screen.
    The graphics background write mode is set to opaque.
    The line mask is set to #FF and the first pixel of lines are plotted.
    The graphics window is not cleared.
    Related entries:
    GRA DEFAULT
    GRA RESET
    SCR INITIALISE
*/
void fw_gra_initialise(void);

/** 63: GRA RESET
    #BBBD
    Reset the Graphics VDU.
    Action:
    Re-initialize the Graphics VDU indirections to their default routines and set default
    modes.
    Entry conditions:
    No conditions.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    Sets the Graphics VDU indirections (GRA PLOT, GRA TEST and GRA LINE) to their
    default routines. V1.1 firmware also sets the graphics background mode to opaque,
    sets the line mask to #FF and sets the first pixel of lines to be plotted.
    Related entries:
    GRA DEFAULT
    GRA INITIALISE
*/
void fw_gra_reset(void);

/** 64: GRA MOVE ABSOLUTE
    #BBC0
    Move to an absolute position.
    Action:
    Move the current position to an absolute position.
    Entry conditions:
    DE contains the required user X coordinate.
    HL contains the required user Y coordinate.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    The new position is given in user coordinates. i.e. Relative to the user origin.
    The new position can be outside the graphics window.
    The Graphics VDU plotting, testing and line drawing routines all move the current
    graphics position to the point (or endpoint) specified automatically.
    Related entries:
    GRA ASK CURSOR
    GRA MOVE RELATIVE
*/
void fw_gra_move_absolute(int16_t x, int16_t y);

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    The fastcall variant may be useful if you already have a reason to
    use the union/struct fw_txt_win_enable to store coordinates.  Else
    it won't save you anything.

    Use the fastcall variant like this:

    fw_gra_x_y_coordinates_t xy = { 85, 63 }; // example values
    fw_gra_move_absolute__fastcall(xy.as_uint32_t);
*/
void fw_gra_move_absolute__fastcall(uint32_t fw_gra_x_y_coordinates_t_asint) __z88dk_fastcall;
void fw_gra_line_absolute(int x, int y);
void fw_gra_plot_absolute(int x, int y);
void fw_gra_test_absolute(int x, int y);

void fw_gra_line_relative(int x, int y);
void fw_gra_plot_relative(int x, int y);
void fw_gra_move_relative(int x, int y);
void fw_gra_test_relative(int x, int y);

void fw_gra_set_pen(unsigned char pencolor);

unsigned char fw_gra_get_pen(void);

void fw_gra_set_paper(unsigned char papercolor);

unsigned char fw_gra_get_paper(void);

void fw_gra_wr_char(char character);

void fw_gra_clear_window(void);
void fw_gra_default(void);

void fw_gra_win_width(int xleft, int xright);
void fw_gra_win_height(int ytop, int ybottom);

#endif /* __FW_GRA_H__ */
