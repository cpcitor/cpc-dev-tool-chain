.module fw_cas_stop_motor

_fw_cas_stop_motor::
	ld	a,l
	call	0xBC74	; CAS STOP MOTOR
	xor	a,a
	ret	nc
	dec	a
	ret
