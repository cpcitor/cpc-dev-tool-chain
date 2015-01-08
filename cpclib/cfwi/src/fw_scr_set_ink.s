.module fw_scr_set_ink

_fw_scr_set_ink::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        ld      d,a
	inc 	hl
	ld      a,(hl)
	ld      b,a
	inc     hl
	ld      a,(hl)
        ld      c,a
        ld      a,d
        call    0xBC32  ; SCR SET INK
        ret
