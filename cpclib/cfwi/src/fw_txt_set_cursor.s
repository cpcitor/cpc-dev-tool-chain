.module fw_txt_set_cursor

_fw_txt_set_cursor::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        call    0xBB6F   ; TXT SET CURSOR
        ret
