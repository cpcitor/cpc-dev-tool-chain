.module fw_txt_set_matrix

_fw_txt_set_matrix::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)          ; character to redefine
        inc     hl
        ld      e,(hl)          ; LSB of buffer holding new character data
        inc     hl
        ld      h,(hl)          ; MSB of buffer holding new character data
        ld      l,e
        call    0xbba8
        ld      a,#0
        ret     nc
        dec     a
        ret
