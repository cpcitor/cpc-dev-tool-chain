.module fw_txt_wr_char

_fw_txt_wr_char::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        call    0xBB5D   ; TXT WR CHAR
        ret
