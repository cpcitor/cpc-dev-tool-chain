.module fw_cas_out_open

_fw_cas_out_open::
	;; hl = pointer to struct fw_cas_open_parameters_t
	ld	c,(hl)		; msb(address of filename)
	inc	hl
	ld	a,(hl)		; lsb(address of filename)
	inc	hl
	ld	b,(hl)		; length of filename
	inc	hl
	ld	e,(hl)		; lsb(buffer)
	inc	hl
	ld	d,(hl)		; msb(buffer)
	inc	hl
	push	hl		; will be used for return value
	ld	l,c
	ld	h,a
	call	0xBC8C		; CAS OUT OPEN
	ld	d,h
	ld	e,l
	pop	hl
	ld	(hl),e		; lsb(header location)
	inc	hl
	ld	(hl),d		; msb(header location)
	ld	a,#0
	jr	z,zero
	inc	a               ; does not affect carry
zero:
	rlca                    ; propagates carry
	ld	l,a
	ret
