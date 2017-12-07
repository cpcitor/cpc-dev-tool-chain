.module fw_txt_get_translate

_fw_txt_get_translate::
	ld 	a,l
	call	0xBB2A   ; TXT GET TRANSLATE
	ld 	l,a
	ret

