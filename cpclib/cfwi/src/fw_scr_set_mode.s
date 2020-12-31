.module fw_scr_set_mode

_fw_scr_set_mode::
        ld      a,l
        jp    0xBC0E  ; TXT SET COLUMN
        ; rather than call + ret
