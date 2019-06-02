.module fw_scr_get_location

_fw_scr_get_location::
	call	0xBC0B   ; SCR GET LOCATION
	ld 	e,a
	ret

