.module fw_mc_screen_offset

_fw_mc_screen_offset::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)		; screen base

	inc	hl
        ld      c,(hl)		; screen offset, LSB

	inc	hl
        ld      h,(hl)		; screen offset, MSB

        ld      l,c		; screen offset, LSB

        jp      0xBD1F  ; MC SCREEN OFFSET
