.module fw_txt_set_pen

_fw_txt_set_pen::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        call    0xBB90   ; TXT SET PEN
        ret
