.module fw_km_get_repeat

_fw_km_get_repeat::
	call	0xBB3C   ; KM GET REPEAT
        ld      l,#0
	ret	z
        dec 	l
        ret

