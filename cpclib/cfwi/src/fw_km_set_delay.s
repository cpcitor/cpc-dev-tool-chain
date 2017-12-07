.module fw_km_set_delay

_fw_km_set_delay::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        inc     hl
        ld      l,(hl)
	ld	h,a
	jp	0xBB3F   ; KM SET DELAY
