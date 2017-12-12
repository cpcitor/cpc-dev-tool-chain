.module fw_txt_get_matrix

_fw_txt_get_matrix::
        call    0xBBA5          ; TXT GET MATRIX
        ld      e,#0
        ret     nc
        dec     e
        ret
