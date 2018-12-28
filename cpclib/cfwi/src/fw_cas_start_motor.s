.module fw_cas_start_motor

_fw_cas_start_motor::
	call	0xBC6E	; CAS START MOTOR
	ld	l,a
	ret	nc
	inc	a
	ret

