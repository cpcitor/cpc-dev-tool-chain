.module fw_scr_set_border

_fw_scr_set_border::
        ld      hl,#2
        add     hl,sp
        ld      c,(hl)
	inc 	hl
	ld      b,(hl)
        jp      0xBC38  ; SCR SET BORDER
        ;rather than call + ret
