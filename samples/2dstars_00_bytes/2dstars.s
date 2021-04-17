        .module 2dstars_asm

        .z80

        .globl _star_pos_byte_l

_show_stars::
        ;; ld bc,#0xbc0c
        ;; out (c),c
        ;; ld bc,#0xbd40
        ;; out (c),c

        ld a,#00

next_frame:
        call #0xbd19

        ld hl, #(_star_pos_byte_l)

next_star:
        ;; read old address
        ld e,(hl)
        inc h
        ld d,(hl)

        ;; erase old star
        ld (de),a

        ;; compute new address
        dec de
        set 6,d

        ;; store new address
        ld (hl),d
        dec h
        ld (hl),e

        ;; draw new star
        dec a
        ld (de),a
        inc a

        ;; next star
        inc l

        djnz next_star

        jr next_frame

        ;ret
