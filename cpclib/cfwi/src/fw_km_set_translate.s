.module fw_txt_set_translate

_fw_txt_set_translate::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        inc     hl
        ld      b,(hl)
	jp	0xBB27   ; TXT SET TRANSLATE

