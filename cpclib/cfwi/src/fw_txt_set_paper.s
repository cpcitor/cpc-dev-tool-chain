.module fw_txt_set_paper

_fw_txt_set_paper::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        call    0xBB96   ; TXT SET PAPER
        ret
