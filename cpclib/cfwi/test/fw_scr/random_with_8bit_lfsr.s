	.module random_with_8bit_lfsr

; z88dk_fast_call -> input value in HL
_random_with_8bit_lfsr::
	ld	a,(hl)		; hl = pointer to value

;  http://doitwireless.com/2014/06/26/8-bit-pseudo-random-number-generator/
	srl	a
	jr	NC, noxor
	xor 	#0xB8
noxor:
	ld	(hl),a
	ld	l,a
	ret

