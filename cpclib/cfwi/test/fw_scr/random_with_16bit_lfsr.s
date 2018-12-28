	.module random_with_16bit_lfsr

; z88dk_fast_call -> input value in HL
_random_with_16bit_lfsr::
	ld	c,(hl)		; hl = pointer to value, c=lsB
	inc 	hl
	ld	a,(hl)		; a=msB

; https://en.wikipedia.org/wiki/Linear-feedback_shift_register#Galois_LFSRs
	srl	a
	rr	c
	jr	NC, noxor
	xor 	#0xB4
noxor:
	ld	(hl),a
	dec	hl
	ld	(hl),c
	ld	l,c
	ld	h,a
	ret
