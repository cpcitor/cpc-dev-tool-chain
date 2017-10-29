.module fw_txt_win_enable

_fw_txt_win_enable::
        ld      hl,#2
        add     hl,sp
        ld      b,(hl)          ; left -> h
        inc     hl
        ld      d,(hl)          ; right
        inc     hl
        ld      c,(hl)          ; top -> l
        inc     hl
        ld      e,(hl)          ; bottom
        ld      h,b
        ld      l,c
        jp    0xBB66   ; TXT WIN ENABLE

