.module fw_txt_output

_fw_txt_output::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        call    0xBB5A   ; TXT OUTPUT
        ret
