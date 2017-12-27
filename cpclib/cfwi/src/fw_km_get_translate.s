.module fw_km_get_translate

_fw_km_get_translate::
	ld 	a,l
	call	0xBB2A   ; KM GET TRANSLATE
	ld 	l,a
	ret

