
; void cfwi_txt_str0_output (const char* s);
; Output a NUL terminated string at the current cursor position
; TESTED

_cfwi_txt_str0_output::
        ld      hl,#2
        add     hl,sp
        ld      e,(hl)
        ld      hl,#3
        add     hl,sp
        ld      d,(hl)

cputs$:
        ld      a,(de)
        cp      #0
        ret     z
        call    0xBB5A
        inc     de
        jr      cputs$
