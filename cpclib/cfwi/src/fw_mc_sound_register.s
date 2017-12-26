.module fw_mc_sound_register

_fw_mc_sound_register::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)		; sound register number

	inc	hl
        ld      c,(hl)		; data

        jp      0xBD34  ; MC SOUND REGISTER
