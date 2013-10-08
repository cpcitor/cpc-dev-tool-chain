	;; Fill the screen using a given byte.
	;; Reasonably fast while keeping very short.

	.globl _fillscreen

_fillscreen_start::
_fillscreen:
	ld	hl,#2
	add	hl,sp
	ld	a,(hl)
	ld	hl,#0xc000
	ld	de,#0x4000
00101$:
00102$:
	ld	(hl),a
	inc	hl
	dec	e
	jr	NZ,00102$
	dec	d
	jr	NZ,00101$
	ret
_fillscreen_end::
