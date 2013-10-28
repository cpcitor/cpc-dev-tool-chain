.module fw_gra_get_pen

_fw_gra_get_pen::
        call    0xBBE1   ; GRA GET PEN
        ld      l,a
        ret
