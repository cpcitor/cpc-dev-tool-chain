.module fw_cas_set_speed

_fw_cas_set_speed::
	pop	af	;; return address
	pop	hl	;; hl = length_of_half_zero_bit
	dec	sp
	pop	bc	;; b = precompensation
	push	af
	ld	a,b
	jp	0xBBC0	; GRA MOVE ABSOLUTE
