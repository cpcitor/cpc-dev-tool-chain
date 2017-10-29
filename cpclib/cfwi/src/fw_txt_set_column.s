.module fw_txt_set_column

_fw_txt_set_column::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        call    0xBB6F   ; TXT SET COLUMN
        ret
