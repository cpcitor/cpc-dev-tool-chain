.module fw_txt_swap_streams

_fw_txt_swap_streams::
        ld      hl,#2
        add     hl,sp
        ld      b,(hl)
        inc     hl
        ld      c,(hl)
        jp      0xBBB7   ; TXT SWAP STREAM
