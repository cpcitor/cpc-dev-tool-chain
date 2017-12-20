.module fw_gra_test_absolute

_fw_gra_test_absolute::
        ld      hl,#2
        add     hl,sp
        ld      e,(hl)

        inc     hl
        ld      d,(hl)

        inc     hl
        ld      a,(hl)

        inc     hl
        ld      h,(hl)

        ld      l,a
        call    0xBBF0  ; GRA TEST ABSOLUTE
        ld      l,a
        ret
