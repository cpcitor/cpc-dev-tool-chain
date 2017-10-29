.module fw_txt_set_row

_fw_txt_set_row::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        call    0xBB72   ; TXT SET ROW
        ret
