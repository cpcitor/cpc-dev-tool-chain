.module fw_km_read_char

_fw_km_read_char::
        call    0xBB09  ; KM READ CHAR
        ld      l,#1
        ret     c
        ld      l,#0
        ret
