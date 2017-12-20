.module fw_gra_move_relative

_fw_gra_move_relative::
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
        jp      0xBBC3   ; GRA MOVE RELATIVE

