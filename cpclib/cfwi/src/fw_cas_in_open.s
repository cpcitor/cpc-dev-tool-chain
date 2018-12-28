.module fw_cas_in_open

_fw_cas_in_open::
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
	call	0xBC77		; CAS IN OPEN
	;; must preserve flags till ...
	ex	(sp),hl		; hl = pointer to struct fw_cas_in_open_parameters_t
				; stack contains HL returned by CAS IN OPEN
	ld	(hl),a		; file type
	inc	hl
	ld	(hl),e		; lsb(data location)
	inc	hl
	ld	(hl),d		; msb(data location)
	inc	hl
	ld	(hl),c		; lsb(logical file length)
	inc	hl
	ld	(hl),b		; msb(logical file length)
	inc	hl
	pop	de		; de = address of buffer
	ld	(hl),e		; lsb(address of buffer)
	inc	hl
	ld	(hl),d		; msb(address of buffer)
	ld	a,#0		; don't xor a,a it would kill the flags
	;; must preserve flags till here
	jr	z,zero
	inc	a
	rlca
zero:
	jr	nc,nocarry
	inc	a
nocarry:
	ld	l,a
	ret
