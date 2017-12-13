.module fw_txt_get_m_table

_fw_txt_get_m_table::
        call    0xBBAE          ; TXT GET M TABLE
        ld      e,a             ; lowest character in table
        ld      d,#0
        ret     nc              ; if no carry, no table, return with d=0
        dec     d
        ret

