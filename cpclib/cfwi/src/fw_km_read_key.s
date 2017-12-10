.module fw_km_read_key

_fw_km_read_key::
        call    0xBB1B  ; KM READ KEY
	ld 	hl,#0000
        ret     nc
        ld      l,a
        dec 	h
        ret
