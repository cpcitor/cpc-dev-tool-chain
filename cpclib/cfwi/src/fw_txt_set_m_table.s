.module fw_txt_set_m_table

_fw_txt_set_m_table::
        ld      hl,#5
        add     hl,sp
        ld      e,(hl)          ; lowest affected character
        dec     hl
        ld      d,(hl)          ; is buffer already initialized? (can also be considered high byte of 16bit DE since anything 256 or more disables the feature)
        dec     hl
        ld      a,(hl)          ; buffer address MSB
        dec     hl
        ld      l,(hl)          ; buffer address LSB
        ld      h,a
        call    0xBBAB          ; TXT SET M TABLE
        ; HL contains the address of the new table, nothing to do.
        ld      e,a             ; lowest character in previous table
        ld      d,#0
        ret     nc              ; if no carry, no previous table, return with d=0
        dec     d
        ret

