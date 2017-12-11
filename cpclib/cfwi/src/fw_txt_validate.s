.module fw_txt_validate

_fw_txt_validate::
        call	0xBB87		; TXT VALIDATE
	ld	e,#0
	ret	c
	dec	e
	ld 	d,b
        ret
