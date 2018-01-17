.module fw_cas_restore_motor

_fw_cas_restore_motor::
	ld	a,l
	call	0xBC74	; CAS RESTORE MOTOR
	xor	a,a
	ret	nc
	dec	a
	ret
