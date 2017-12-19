#ifndef  __FW_GRA_H__
#define __FW_GRA_H__

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

void fw_gra_line_absolute(int x, int y);
void fw_gra_plot_absolute(int x, int y);
void fw_gra_move_absolute(int x, int y);
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
