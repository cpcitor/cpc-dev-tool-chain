.module fw_txt_rd_char

_fw_txt_rd_char::
	call 	0xbb60		; TXT RD CHAR
	ld	h,#0
	ret 	nc
	ld	l,a
        dec	h
        ret
