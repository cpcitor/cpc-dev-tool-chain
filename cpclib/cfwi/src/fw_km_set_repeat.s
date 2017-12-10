.module fw_km_set_repeat

_fw_km_set_repeat::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        inc     hl
        ld      b,(hl)
        jp      0xBB39   ; KM SET REPEAT

