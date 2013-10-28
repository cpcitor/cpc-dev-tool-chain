.module fw_gra_set_pen

_fw_gra_set_pen::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        call    0xBBDE   ; GRA SET PEN
        ret
