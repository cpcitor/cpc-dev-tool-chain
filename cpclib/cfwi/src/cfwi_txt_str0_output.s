
; void cfwi_txt_str0_output (const char* s);
; Output a NUL terminated string at the current cursor position
; TESTED

_cfwi_txt_str0_output::
        ld      hl,#2
        add     hl,sp
        ld      e,(hl)
        inc     hl
        ld      d,(hl)

cputs$:
        ld      a,(de)
        cp      #0
        ret     z
        call    0xBB5A 		; TXT OUTPUT
        inc     de
        jr      cputs$
