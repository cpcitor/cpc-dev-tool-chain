.module fw_km_get_shift

_fw_km_get_shift::
	ld 	a,l
	call	0xBB30   ; KM GET SHIFT
	ld 	l,a
	ret

