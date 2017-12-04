.module fw_txt_set_m_table

_fw_txt_set_m_table::
        ld      hl,#5
        add     hl,sp
        ld      e,(hl) 		; buffer size
        dec     hl
        ld      d,(hl)		; is buffer already initialized?
	dec	hl
	ld	a,(hl)		; buffer address MSB
	dec	hl
	ld	l,(hl)		; buffer address LSB
	ld	h,a
        jp    0xBBAB   ; TXT SET M TABLE
