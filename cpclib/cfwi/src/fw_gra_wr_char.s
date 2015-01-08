.module fw_gra_wr_char

_fw_gra_wr_char::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        call    0xBBFC   ; GRA WR CHAR
        ret
