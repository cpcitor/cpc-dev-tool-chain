.module fw_scr_set_mode

_fw_scr_set_mode::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        call    0xBC0E  ; TXT SET COLUMN
        ret
