.module fw_txt_get_pen

_fw_txt_get_pen::
        call    0xBB93   ; TXT GET PEN
        ld      l,a
        ret
