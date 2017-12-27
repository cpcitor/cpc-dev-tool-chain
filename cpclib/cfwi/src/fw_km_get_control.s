.module fw_km_get_control

_fw_km_get_control::
	ld 	a,l
	call	0xBB36   ; TXT GET CONTROL
	ld 	l,a
	ret

