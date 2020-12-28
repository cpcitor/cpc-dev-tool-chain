        .globl _screen_address_processing__table_h

_cdtc_screen_basic__plot::
        ;; H = y, L = x

        ;; Compute mask

        ;; How many times should we bitshift to get the mask?
        ld a, #3
        and l
        ld b,a
        inc b                   ; simpler+faster to inc b and make one loop step than make more jumps.

        ;; compute mask, loop 1 to 4 times.
        ld a,#0x11
cdtc_screen_basic__plot_mask_loop:
        rrca
        djnz cdtc_screen_basic__plot_mask_loop
        ;; ok mask is in a
        ld b,a
        ;; ok mask is in b

        ;; now compute address
        ld d, #_screen_address_processing__table_h >> 8 ; MSB table
        ld e, h                 ; y
        ld a,(de)               ; read MSB
        ld h,a

        dec d                   ; LSB table
        ld a,(de)               ; read LSB

        ;; address to add is quarter of x
        srl l
        srl l
        add l                   ; add x
        jr nc, cdtc_screen_basic__plot_no_carry
        inc h
cdtc_screen_basic__plot_no_carry:
        ld l,a

        ;; ok we have address in hl, now add our pixel
        ld a,(hl)
        or b
        ld (hl),a
        ret
