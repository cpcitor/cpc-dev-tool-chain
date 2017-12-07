.module fw_txt_get_shift

_fw_txt_get_shift::
	ld 	a,l
	call	0xBB30   ; TXT GET SHIFT
	ld 	l,a
	ret

