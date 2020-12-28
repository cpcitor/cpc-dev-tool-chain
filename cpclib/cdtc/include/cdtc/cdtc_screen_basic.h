#ifndef __cdtc_screen_basic_h__
#define __cdtc_screen_basic_h__

/** **************** What is this module? ****************

    This module provides facilities for screen access: configure the
    CRTC, write text and graphics.

    It covers cases not covered by the firmware: arbitrary width
    between 8 and 504 pixels, height between 8 and 288+ pixels,
    especially the common 256x256 mode,

    It is called "basic" because it is focused on keeping things
    simple and fast.

    Some things it handles:

    - compute a table for fast access (must be aligned to 256 bytes, 256 bytes low address, then 256 bytes high address)
    - very fast pixel access routine
    - very fast text routine

    Some limitations:

    - to keep routines fast and compilation/linking simple, routines are assumed to be used with only one screen.  If you use several, you can duplicate relevant files and change symbol names.
    - to keep routines fastest, assuming no hardware scrolling
    - to keep routines fastest, assuming
    - not all screen sizes are supported by all features.  It's
      still a CPC!

    What is fully supported is the 256x256 mode.

    Some things it does not handle:

    - no 32k screens
    - no screen split

    **************** How do I use this module? ****************

    -Step 1: in your project, create an assembler file, suggested name
     is `cdtc_screen_parameters.s`, with the parameters of the screen
     you want, and the location of table in memory.  See below for
     details of its content.

    -Step 2: in C files that use this module, make sure to `#include
     <cdtc_screen_basic.h>`.

    -Step 3a: call `cdtc_screen_basic__configure_crtc()` from C, (or
     from ASM call `_cdtc_screen_basic__configure_crtc`).

    -Step 3b: ensure your code calls
     `cdtc_screen_basic__table_lh__fill()` before any text or graphics
     routine is called.

    -At will, call from ASM _cdtc_screen_basic_plot from ASM with H=y
     and L=x, or, from C cdtc_screen_basic_plot(arg) where you'll
     probably use uint16_t_as_uint8_t_pair_t to store coordinates.

    When compiling and linking, only the C routines that you actually
    used are included in the generated binary.



    Below are examples, you can use your own values.

    All units are CRTC units, a.k.a MODE 1 character width and height.

    Example 1 : 320x200 screen, same as firmware.

_cdtc_screen_basic__table_lh == 0x7E00
_screen_first_byte_address == 0xC000
_crtc_r1_disp_width_chars == 40    ; 40 chars, 80 bytes, 320-pixel wide screen, the standar
_crtc_r6_disp_height_chars == 25   ; 25 chars, 200-pixel high screen
_crtc_r9_plus_1_disp_lines_per_char_height == 8 ; standard value 8, r9 == 7
; Don't change lines below
_screen_width_in_bytes == _crtc_r1_disp_width_chars * 2
_screen_total_lines == _crtc_r6_disp_height_chars * _crtc_r9_plus_1_disp_lines_per_char_height

    Example 2 : 256x256 screen, the one primarily used and most tested

_cdtc_screen_basic__table_lh == 0x100 ; if you don't return to BASIC, you can overwrite 0x100-0x2FF
_screen_first_byte_address == 0xC000
_crtc_r1_disp_width_chars == 32    ; 32 chars, 64 bytes, 256-pixel wide screen (standard is 40)
_crtc_r6_disp_height_chars == 32   ; 32 chars, 256-pixel high screen
_crtc_r9_plus_1_disp_lines_per_char_height == 8 ; standard value 8, r9 == 7
; Don't change lines below
_screen_width_in_bytes == _crtc_r1_disp_width_chars * 2
_screen_total_lines == _crtc_r6_disp_height_chars * _crtc_r9_plus_1_disp_lines_per_char_height

    Example 3 : 256x192 screen (with much room for hardware scrolling that is not supported yet, 8 blocks of 512 bytes of unused memory)

_cdtc_screen_basic__table_lh == 0x7E00
_screen_first_byte_address == 0xC000
_crtc_r1_disp_width_chars == 32    ; 32 chars, 64 bytes, 256-pixel wide screen (standard is 40)
_crtc_r6_disp_height_chars == 24   ; 24 chars, 192-pixel high screen
_crtc_r9_plus_1_disp_lines_per_char_height == 8 ; standard value 8, r9 == 7
; Don't change lines below
_screen_width_in_bytes == _crtc_r1_disp_width_chars * 2
_screen_total_lines == _crtc_r6_disp_height_chars * _crtc_r9_plus_1_disp_lines_per_char_height

    Example 4 : 256x192 screen (occupies a solid 12288 bytes, useful to use the rest of memory in one contiguous block) -- NOT SUPPORTED YET, routine does not yet compute value for register 4 nor set it.

_cdtc_screen_basic__table_lh == 0xF000 ; put table just after screen, because why not
_screen_first_byte_address == 0xC000
_crtc_r1_disp_width_chars == 32    ; 32 chars, 64 bytes, 256-pixel wide screen (standard is 40)
_crtc_r6_disp_height_chars == 32   ; 32 chars but of height 6, 192-pixel high screen
_crtc_r9_plus_1_disp_lines_per_char_height == 6 ; unusual value 6 instead of standard value 8, r9 == 5
; Don't change lines below
_screen_width_in_bytes == _crtc_r1_disp_width_chars * 2
_screen_total_lines == _crtc_r6_disp_height_chars * _crtc_r9_plus_1_disp_lines_per_char_height

Adjusting from these examples, you can create your own mode.

 */


/** Fill a table

    You must ensure that
 */
void cdtc_screen_basic__table_lh__fill( void );


void cdtc_screen_basic__configure_crtc( void );


#include <stdint.h>

void cdtc_screen_basic__plot( uint16_t coords ) __z88dk_fastcall;

#endif /* __cdtc_screen_basic_h__ */
