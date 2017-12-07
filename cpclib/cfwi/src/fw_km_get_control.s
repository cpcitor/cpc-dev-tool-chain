.module fw_txt_get_control

_fw_txt_get_control::
	ld 	a,l
	call	0xBB36   ; TXT GET CONTROL
	ld 	l,a
	ret

