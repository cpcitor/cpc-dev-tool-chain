        .module 2dstars_asm

        .z80

        .globl _star_pos_byte_l
        .globl _setborder

.include "cdtc/cpc_hardware_colors.s"

_show_stars::
        ;; ld bc,#0xbc0c
        ;; out (c),c
        ;; ld bc,#0xbd40
        ;; out (c),c

        ld a,#00

next_frame:
        ld l,#hardware_color_r0_g0_b0_black
        call _setborder
        call #0xbd19
        ld l,#hardware_color_r0_g0_b2_bright_blue
        call _setborder

        ld hl, #(_star_pos_byte_l)
        ld b,#0x0
next_star:
        ;; read old address
        ld e,(hl)
        inc h
        ld d,(hl)

        ;; erase old star
        xor a
        ld (de),a

        ;; compute new address
        dec de
        set 6,d

        ;; store new address
        ld (hl),d
        dec h
        ld (hl),e

        ;; draw new star
        ld a,b
        ld (de),a

        ;; next star
        inc l

        djnz next_star

        jr next_frame

        ;ret
