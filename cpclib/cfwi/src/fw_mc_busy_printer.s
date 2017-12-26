.module fw_mc_busy_printer

_fw_mc_busy_printer::
        call    0xBD2E          ; MC BUSY PRINTER
        ld      l,#0
        ret     nc
        dec     l
        ret
