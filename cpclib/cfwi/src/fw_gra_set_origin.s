.module fw_gra_set_origin

_fw_gra_set_origin::
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
        jp      0xBBC9   ; GRA SET ORIGIN
