.module fw_gra_set_paper

_fw_gra_set_paper::
        ld      hl,#2
        add     hl,sp
        ld      a,(hl)
        call    0xBBE4   ; GRA SET PAPER
        ret
