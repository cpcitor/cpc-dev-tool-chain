.module fw_mc_start_program

_fw_mc_start_program::
        ld      hl,#2
        add     hl,sp
        ld      c,(hl)		; ROM selection

	inc	hl
        ld      a,(hl)		; entry point, LSB

	inc	hl
        ld      h,(hl)		; entry point, MSB

        ld      l,a		; entry point, LSB

        jp      0xBD16  ; MC START PROGRAM
