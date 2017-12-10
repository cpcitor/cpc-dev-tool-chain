.module fw_txt_set_shift

_fw_txt_set_shift::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        inc     hl
        ld      b,(hl)
	jp	0xBB2D   ; TXT SET SHIFT

