.module fw_km_set_expand

_fw_km_set_expand::
        ld      hl,#2
        add     hl,sp
        ld      b,(hl)		; expansion token for the expansion to set
        inc     hl
        ld      c,(hl)		; length of the string
	inc	hl
	ld	a,(hl)		; address of the string, LSB
	inc	hl
	ld	h,(hl)		; address of the string, MSB
	ld	l,a
	call 	0xbb0f
        ld      l,#0
        ret     c
        inc 	l
        ret
