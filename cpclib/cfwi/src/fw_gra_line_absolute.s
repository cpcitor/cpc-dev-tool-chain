.module fw_gra_line_absolute

_fw_gra_line_absolute::
        ld      hl,#2
        add     hl,sp
        ld      e,(hl)

	inc	hl
        ld      d,(hl)

	inc	hl
        ld      a,(hl)

        inc     hl
        ld      h,(hl)

        ld      l,a
        jp      0xBBF6  ; GRA LINE ABSOLUTE

