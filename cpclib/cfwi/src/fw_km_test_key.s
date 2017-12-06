.module fw_km_test_key

_fw_km_test_key::
	ld 	a,l
        call    0xBB1E  ; KM TEST KEY
	ld	h,c
        ld      l,#0
	ret	z
        dec 	l
        ret
