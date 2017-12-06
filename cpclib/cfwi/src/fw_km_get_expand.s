.module fw_km_get_expand

_fw_km_get_expand::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)		; expansion token for the expansion to set
        inc     hl
        ld      l,(hl)		; length of the string
	call 	0xbb12
	ld 	l,a
        ld      h,#0
        ret     c
        dec	h
        ret
