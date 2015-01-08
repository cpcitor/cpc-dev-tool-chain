.module fw_scr_set_border

_fw_scr_set_border::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        ld      c,a
	inc 	hl
	ld      a,(hl)
	ld      b,a
        call    0xBC38  ; SCR SET BORDER
        ret
