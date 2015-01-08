.module fw_gra_win_height

_fw_gra_win_height::
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
        call    0xBBD2  ; GRA WIN HEIGHT
        ret
