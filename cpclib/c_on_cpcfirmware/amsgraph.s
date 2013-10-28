;*****************************************************************************/
; AMSGRAPH.S - A small graphics library for the Amsrad CPC
; For use with the Small Devices C Compiler
;
; (c) 2003 H. Hansen
;*****************************************************************************/
;
;Functions:
;void draw(int x, int y)
;void plot(int x, int y)
;void move(int x, int y)
;void gpen(unsigned char pencolor)
;unsigned char getgpen(void)

.globl _draw

;void draw(int x, int y);
;Draws a line from the current graphics curser position to coordinates x,y

_draw::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        ld      e,a

        ld      hl,#3
        add     hl,sp
        ld      a,(hl)
        ld      d,a

        ld      hl,#4
        add     hl,sp
        ld      a,(hl)
        ld      c,a

        ld      hl,#5
        add     hl,sp
        ld      a,(hl)
        ld      b,a

        ld      h,b
        ld      l,c
        call    0xBBF6  ; GRA LlNE ABSOLUTE
        ret

.globl _plot

;void plot(int x, int y)
;Plot a point on x,y int the current graphics color!

_plot::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        ld      e,a
        ld      hl,#3
        add     hl,sp
        ld      a,(hl)
        ld      d,a

        ld      hl,#4
        add     hl,sp
        ld      a,(hl)
        ld      c,a

        ld      hl,#5
        add     hl,sp
        ld      a,(hl)
        ld      b,a

        ld      h,b
        ld      l,c
        call    0xBBEA  ; GRA PLOT ABSOLUTE

        ret

.globl _gpen

;void gpen(unsigned char pencolor)
;Sets the graphics pen!

_gpen::

        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        call    0xBBDE   ; GRA SET PEN
        ret

.globl _getgpen

;unsigned char getgpen(void)
;Returns the used graphics pen

_getgpen::
        call    0xBBE1   ; GRA GET PEN
        ld      l,a
        ret

.globl _move

;void move(int x, int y)
;Moves the graphic curser to coordinates x,y

_move::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        ld      e,a

        ld      hl,#3
        add     hl,sp
        ld      a,(hl)
        ld      d,a

        ld      hl,#4
        add     hl,sp
        ld      a,(hl)
        ld      c,a

        ld      hl,#5
        add     hl,sp
        ld      a,(hl)
        ld      b,a

        ld      h,b
        ld      l,c
        call    0xBBC0   ; GRA MOVE ABSOLUTE
        ret
