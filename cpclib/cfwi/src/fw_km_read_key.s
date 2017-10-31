.module fw_km_read_key

_fw_km_read_key::
        call    0xBB1B  ; KM READ KEY
        ld      l,a
        ret     c
        ld      l,#0
        ret
