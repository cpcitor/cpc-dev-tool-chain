.module fw_cas_stop_motor

_fw_cas_stop_motor::
	call	0xBC71	; CAS STOP MOTOR
	ld	l,a
	ret	nc
	inc	a
	ret

