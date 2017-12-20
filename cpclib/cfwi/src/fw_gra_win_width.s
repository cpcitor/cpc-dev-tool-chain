.module fw_gra_win_width

_fw_gra_win_width::
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
        jp      0xBBCF  ; GRA WIN WIDTH

