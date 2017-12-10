.module fw_txt_set_control

_fw_txt_set_control::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        inc     hl
        ld      b,(hl)
	jp	0xBB33   ; TXT SET CONTROL

