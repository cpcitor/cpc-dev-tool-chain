.module fw_km_read_char

_fw_km_read_char::
        call    0xBB09  ; KM READ CHAR
	ld 	h,#0
        ret     c
        dec 	h
        ld      l,a
        ret
