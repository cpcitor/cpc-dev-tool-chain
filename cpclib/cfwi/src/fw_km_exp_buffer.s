.module fw_km_exp_buffer

_fw_km_exp_buffer::
        ld      hl,#2
        add     hl,sp
        ld      e,(hl) 		; buffer address LSB
        inc     hl
        ld      d,(hl)		; buffer address MSB
	inc	hl
	ld	a,(hl)		; buffer bytecount LSB
	inc	hl
	ld	h,(hl)		; buffer bytecount MSB
	ld	l,a
	call 	0xbb15
	ld 	l,#0
        ret     c
        dec	l
        ret

