#ifndef  __FW_GRA_H__
#define __FW_GRA_H__

#include <stdint.h>

/** This structure (union/struct actually) was introduced to decode output of
    fw_gra_ask_cursor().
    
    It is also a natural structure to hold coordinates.

    If you use is for that storage purpose, then it is natural to also
    use the fastcall variants of drawing calls that expect X and Y
    coordinates.
*/
typedef union fw_gra_x_y_coordinates_t
{
	struct
	{
		int16_t y;
		int16_t x;
	};
	uint32_t as_uint32_t;
} fw_gra_x_y_coordinates_t;

/** This structure (union/struct actually) was introduced to decode output of
    fw_gra_get_w_width().
    
    It is also a natural structure to hold coordinates.

    If you use is for that storage purpose, then it is natural to also
    use the fastcall variants of drawing calls that expect X and X
    coordinates, e.g. GRA WIN WIDTH.
*/
typedef union fw_gra_x_x_coordinates_t
{
	struct
	{
		int16_t x1;
		int16_t x2;
	};
	uint32_t as_uint32_t;
} fw_gra_x_x_coordinates_t;

/** This structure (union/struct actually) was introduced to decode output of
    fw_gra_get_w_width().
    
    It is also a natural structure to hold coordinates.

    If you use is for that storage purpose, then it is natural to also
    use the fastcall variants of drawing calls that expect Y and Y
    coordinates, e.g. GRA WIN WIDTH.
*/
typedef union fw_gra_y_y_coordinates_t
{
	struct
	{
		int16_t y1;
		int16_t y2;
	};
	uint32_t as_uint32_t;
} fw_gra_y_y_coordinates_t;

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
    use the union/struct fw_gra_x_y_coordinates_t to store
    coordinates.  Else it won't save you anything.

    Use the fastcall variant like this:

    fw_gra_x_y_coordinates_t xy = { 85, 63 }; // example values
    fw_gra_move_absolute__fastcall(xy.as_uint32_t);
*/
void fw_gra_move_absolute__fastcall(uint32_t fw_gra_x_y_coordinates_t_asint) __z88dk_fastcall;
void fw_gra_line_absolute(int x, int y);

/** 65: GRA MOVE RELATIVE
    #BBC3
    Move relative to current position.
    Action:
    Move the current position to relative to its current position.
    Entry conditions:
    DE contains a signed X offset.
    HL contains a signed Y offset.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    The new position can be outside the graphics window.
    The Graphics VDU plotting, testing and line drawing routines all move the current
    graphics position to the point (or endpoint) specified automatically.
    Related entries:
    GRA ASK CURSOR
    GRA MOVE ABSOLUTE
*/
void fw_gra_move_relative(int16_t x, int16_t y);

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    The fastcall variant may be useful if you already have a reason to
    use the union/struct fw_gra_x_y_coordinates_t to store
    coordinates.  Else it won't save you anything.

    Use the fastcall variant like this:

    fw_gra_x_y_coordinates_t xy = { 85, 63 }; // example values
    fw_gra_move_relative__fastcall(window_def.as_uint32_t);
*/
void fw_gra_move_relative__fastcall(uint32_t fw_gra_x_y_coordinates_t_asint) __z88dk_fastcall;

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    #### CFWI-specific information: ####

    Since C cannot handle carry flag, the information is returned like this:

    fw_gra_x_y_coordinates_t xy;
    xy.as_uint32_t = fw_gra_ask_cursor();
    printf("x=%d, y=%d", xy.x, xy.y);

    66: GRA ASK CURSOR
    #BBC6
    Get the current position.
    Action:
    Ask where the current graphics position is.
    Entry conditions:
    No conditions.
    Exit conditions:
    DE contains the user X coordinate.
    HL contains the user Y coordinate.
    AF corrupt.
    All other registers preserved.
    Notes:
    The new position is given in user coordinates. i.e. Relative to the user origin.
    The Graphics VDU plotting, testing and line drawing routines all move the current
    graphics position to the point (or endpoint) specified automatically. Thus, the position
    returned is probably where the last point was plotted or tested.
    Related entries:
    GRA MOVE ABSOLUTE
    GRA MOVE RELATIVE
*/
uint32_t fw_gra_ask_cursor(void);

/** 67: GRA SET ORIGIN
    #BBC9
    Set the origin of the user coordinates.
    Action:
    Set the location of the user origin and move the current position there.
    Entry conditions:
    DE contains the standard X coordinate of the origin.
    HL contains the standard Y coordinate of the origin.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    The origin position is given is standard coordinates in which (0,0) is the bottom left
    corner of the screen.
    The default origin position is at (0,0). Whenever the screen mode is changed, by
    calling SCR SET MODE, the origin is restored to its default position.
    Related entries:
    GRA FROM USER
    GRA GET ORIGIN
*/
void fw_gra_set_origin(int16_t x, int16_t y);

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    The fastcall variant may be useful if you already have a reason to
    use the union/struct fw_gra_x_y_coordinates_t to store
    coordinates.  Else it won't save you anything.

    Use the fastcall variant like this:

    fw_gra_x_y_coordinates_t xy = { 85, 63 }; // example values
    fw_gra_set_origin__fastcall(xy.as_uint32_t);
*/
void fw_gra_set_origin__fastcall(uint32_t fw_gra_x_y_coordinates_t_asint) __z88dk_fastcall;

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    #### CFWI-specific information: ####

    Since C cannot handle carry flag, the information is returned like this:

    fw_gra_x_y_coordinates_t xy;
    xy.as_uint32_t = fw_gra_get_origin();
    printf("x=%d, y=%d", xy.x, xy.y);

    68: GRA GET ORIGIN
    #BBCC
    Get the origin of the user coordinates.
    Action:
    Ask where the user coordinate origin is located.
    Entry conditions:
    No conditions.
    Exit conditions:
    DE contains the standard X coordinate of the origin.
    HL contains the standard Y coordinate of the origin.
    All other registers preserved.
    Notes:
    The origin position is given is standard coordinates in which (0,0) is the bottom left
    corner of the screen.
    Related entries:
    GRA SET ORIGIN
*/
uint32_t fw_gra_get_origin(void);

/** 69: GRA WIN WIDTH
    #BBCF
    Set the right and left edges of the graphics window.
    Action:
    Set the horizontal position of the graphics window. The left and right edges are
    respectively the first and last points that lie inside the window horizontally.
    Entry conditions:
    DE contains the standard X coordinate of one edge.
    HL contains the standard X coordinate of the other edge.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All registers preserved.
    Notes:
    The window edges are given in standard coordinates in which (0,0) is the bottom left
    corner of the screen and coordinates are signed 16 bit numbers.
    The left edge of the window is deemed to be the smaller of the two edges supplied.
    The window will be truncated, if necessary, to make it fit the screen. The edges are
    moved to screen byte boundaries so that the window only contains whole bytes (the
    left edge is moved left, the right edge is moved right). This moves the coordinates of
    the edges as follows in the various modes:
    Mode Left Edge Right Edge
    0 Multiple of 2 Multiple of 2 minus 1
    1 Multiple of 4 Multiple of 4 minus 1
    2 Multiple of 8 Multiple of 8 minus 1
    The default window covers the whole screen. Whenever the screen mode is changed
    the window is restored to its default size.
    All Graphics VDU point plotting and line drawing routines test whether the points
    they are about to plot lie inside the window; if they are not then the points are not
    plotted.
    Related entries:
    GRA GET W WIDTH
    GRA WIN HEIGHT
*/
void fw_gra_win_width(int16_t x1, int16_t x2);

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    The fastcall variant may be useful if you already have a reason to
    use the union/struct fw_gra_x_x_coordinates_t to store
    coordinates.  Else it won't save you anything.

    Use the fastcall variant like this:

    fw_gra_x_x_coordinates_t xx = { 85, 63 }; // example values
    fw_gra_win_width__fastcall(xx.as_uint32_t);
*/
void fw_gra_win_width__fastcall(int32_t fw_gra_x_x_coordinates_t_asint) __z88dk_fastcall;

/** 70: GRA WIN HEIGHT
    #BBD2
    Set the top and bottom edges of the graphics window.
    Action:
    Set the vertical position of the graphics window. The top and bottom edges are
    respectively the last and first points that lie inside the window vertically.
    Entry conditions:
    DE contains the standard Y coordinate of one edge.
    HL contains the standard Y coordinate of the other edge.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All registers preserved.
    Notes:
    The window edges are given in standard coordinates in which (0,0) is the bottom left
    corner of the screen and coordinates are signed 16 bit numbers.
    The top edge will be deemed to be the higher of the two edges supplied.
    The window will be truncated, if necessary, to make it fit the screen. The edges will
    be moved to lie on screen line boundaries so that only whole screen lines are included
    in the window (the top edge will be moved up, the bottom edge will be moved down).
    This moves the bottom edge to an even coordinate and the top edge to an odd
    coordinate.
    The default window covers the whole screen. Whenever the screen mode is changed
    the window is restored to its default size.
    All Graphics VDU point plotting and line drawing routines test whether the points
    they are about to plot lie inside the window; if they are not then the points are not
    plotted.
    Related entries:
    GRA GET W HEIGHT
    GRA WIN WIDTH
*/
void fw_gra_win_height(int16_t y1, int16_t y2);

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    The fastcall variant may be useful if you already have a reason to
    use the union/struct fw_gra_y_y_coordinates_t to store
    coordinates.  Else it won't save you anything.

    Use the fastcall variant like this:

    fw_gra_y_y_coordinates_t yy = { 85, 63 }; // example values
    fw_gra_win_height__fastcall(yy.as_uint32_t);
*/
void fw_gra_win_height__fastcall(int32_t fw_gra_y_y_coordinates_t_asint) __z88dk_fastcall;

/** 73: GRA CLEAR WINDOW
    #BBDB
    Clear the graphics window.
    Action:
    Clear the graphics window to the graphics paper ink.
    Entry conditions:
    No conditions.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    The current graphics position is moved to the origin of the user coordinates.
    Related entries:
    GRA SET PAPER
    GRA WIN HEIGHT
    GRA WIN WIDTH
    SCR CLEAR
    TXT CLEAR WINDOW
*/
void fw_gra_clear_window(void);

/** 74: GRA SET PEN
    #BBDE
    Set the graphics plotting ink.
    Action:
    Set the graphics pen ink. This is the ink by the Graphics VDU for plotting points,
    drawing lines and writing characters.
    Entry conditions:
    A contains the required ink.
    Exit conditions:
    AF corrupt.
    All other registers preserved.
    Notes:
    The ink is masked to bring it in the range of inks for the current screen mode. In mode
    0 the mask is #0F, in mode 1 it is #03 and in mode 2 it is #01.
    In V1.1 firmware the graphics pen ink is taken to delimit the edge of the area to fill
    when flood filling areas of the screen.
    Related entries:
    GRA GET PEN
    GRA SET PAPER
    SCR SET INK
    TXT SET PEN
*/
void fw_gra_set_pen(unsigned char pencolor) __z88dk_fastcall;

/** 75: GRA GET PEN
    #BBE1
    Get the current graphics plotting ink.
    Action:
    Ask what the current graphics pen ink is set to. This is the ink used by the Graphics
    VDU for plotting points, drawing lines and writing characters.
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
    GRA SET PEN
    SCR GET INK
    TXT GET PEN
*/
unsigned char fw_gra_get_pen(void);

/** 76: GRA SET PAPER
    #BBE4
    Set the graphics background ink.
    Action:
    Set the graphics paper ink.
    Entry conditions:
    A contains the required ink.
    Exit conditions:
    AF corrupt.
    All registers preserved.
    Notes:
    The ink is masked to bring it in the range of inks for the current screen mode. In mode
    0 the mask is #0F, in mode 1 it is #03 and in mode 2 it is #01.
    The paper ink is the ink that is used for clearing the graphics window, and writing the
    background to characters. It is assumed to cover everywhere outside the graphics
    window when testing points.
    In V1.1 firmware the graphics paper ink is used to plot pixels corresponding to a zero
    bit in the line mask when drawing lines.
    Related entries:
    GRA GET PAPER
    GRA SET PEN
    SCR GET INK
    TXT SET PAPER
*/
void fw_gra_set_paper(uint8_t papercolor) __z88dk_fastcall;

/** 77: GRA GET PAPER
    #BBE7
    Get the current graphics background ink.
    Action:
    Ask what the current graphics paper ink is set to.
    Entry conditions:
    No conditions.
    Exit conditions:
    A contains the ink.
    Flags corrupt.
    All other registers preserved.
    Notes:
    The paper ink is the ink that is used for clearing the graphics window, and writing
    background to characters. It is assumed to cover everywhere outside the graphics
    window when testing points.
    Related entries:
    GRA GET PEN
    GRA SET PAPER
    SCR GET INK
    TXT GET PAPER
*/
unsigned char fw_gra_get_paper(void);

/** 78: GRA PLOT ABSOLUTE
    #BBEA
    Plot a point at an absolute position.
    Action:
    The current graphic position is moved to the position supplied. If this lies inside the
    graphics window then the point is plotted in the current graphics pen ink using the
    current graphics write mode. If the point lies outside the graphics window then no
    action is taken.
    Entry conditions:
    DE contains the user X coordinate to plot at.
    HL contains the user Y coordinate to plot at.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    The position to plot at is given in user coordinates. i.e. Relative to the user origin.
    This routine calls the GRA PLOT indirection to plot the point. In its turn GRA PLOT
    calls the SCR WRITE indirection to set the pixel (if it is in the window).
    Related entries:
    GRA PLOT
    GRA PLOT RELATIVE
    GRA TEST ABSOLUTE
*/
void fw_gra_plot_absolute(int16_t x, int16_t y);

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    The fastcall variant may be useful if you already have a reason to
    use the union/struct fw_gra_x_y_coordinates_t to store coordinates.  Else
    it won't save you anything.

    Use the fastcall variant like this:

    fw_gra_x_y_coordinates_t xy = { 85, 63 }; // example values
    fw_gra_plot_absolute__fastcall(xy.as_uint32_t);
*/
void fw_gra_plot_absolute__fastcall(uint32_t fw_gra_x_y_coordinates_t_asint) __z88dk_fastcall;

/** 79: GRA PLOT RELATIVE
    #BBED
    Plot a point relative to the current position.
    Action:
    The current graphic position is moved to the position supplied. If this lies inside the
    graphics window then the point is plotted in the current graphics pen ink using the
    current graphics write mode. If the point lies outside the graphics window then no
    action is taken.
    Entry conditions:
    DE contains a signed X offset.
    HL contains a signed Y offset.
    Exit conditions:
    AF, BC, DE and HL corrupt.
    All other registers preserved.
    Notes:
    The position to plot at is given in relative coordinates. i.e. Relative to the current
    graphics position.
    This routine calls the GRA PLOT indirection to plot the point. In its turn GRA PLOT
    calls the SCR WRITE indirection to set the pixel (if it is in the window).
    Related entries:
    GRA PLOT
    GRA PLOT ABSOLUTE
    GRA TEST RELATIVE
*/
void fw_gra_plot_relative(int16_t x, int16_t y);

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    The fastcall variant may be useful if you already have a reason to
    use the union/struct fw_gra_x_y_coordinates_t to store coordinates.  Else
    it won't save you anything.

    Use the fastcall variant like this:

    fw_gra_x_y_coordinates_t xy = { 85, 63 }; // example values
    fw_gra_plot_relative__fastcall(xy.as_uint32_t);
*/
void fw_gra_plot_relative__fastcall(uint32_t fw_gra_x_y_coordinates_t_asint) __z88dk_fastcall;

/** 80: GRA TEST ABSOLUTE
    #BBF0
    Test a point at an absolute position.
    Action:
    The current graphic position is moved to the position supplied. If this lies inside the
    graphics window then the pixel is read from the screen and the ink it is set to is
    decoded and returned. If the position lies outside the graphics window then the
    current paper ink is returned.
    Entry conditions:
    DE contains the user X coordinate to test at.
    HL contains the user Y coordinate to test at.
    Exit conditions:
    A contains the ink of the specified point (or the graphics paper ink).
    BC, DE, HL and flags corrupt.
    All other registers preserved.
    Notes:
    The position to test is given in user coordinates. i.e. Relative to the user origin.
    This routine calls the GRA TEST indirection to test the point. In its turn GRA TEST
    calls the SCR READ indirection to test the pixel (if it is in the window).
    Related entries:
    GRA PLOT ABSOLUTE
    GRA TEST
    GRA TEST RELATIVE
*/
void fw_gra_test_absolute(int16_t x, int16_t y);

/** WARNING DONE BUT UNTESTED, MIGHT NOT WORK

    The fastcall variant may be useful if you already have a reason to
    use the union/struct fw_gra_x_y_coordinates_t to store coordinates.  Else
    it won't save you anything.

    Use the fastcall variant like this:

    fw_gra_x_y_coordinates_t xy = { 85, 63 }; // example values
    fw_gra_test_absolute__fastcall(xy.as_uint32_t);
*/
void fw_gra_test_absolute__fastcall(uint32_t fw_gra_x_y_coordinates_t_asint) __z88dk_fastcall;

void fw_gra_line_relative(int x, int y);
void fw_gra_test_relative(int x, int y);





void fw_gra_wr_char(char character);

void fw_gra_default(void);

#endif /* __FW_GRA_H__ */
