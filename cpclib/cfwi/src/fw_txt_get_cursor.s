.module fw_txt_get_cursor

_fw_txt_get_cursor::
        call    0xBB78  ; TXT GET CURSOR
        ld      e,a
        ret
