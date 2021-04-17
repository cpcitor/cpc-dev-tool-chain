        .module 2dstars_asm

        .z80

        .globl _star_4pixelsperframe_pos_byte_l
        .globl _star_1pixel_perframe_pos_byte_l
        .globl _setborder

.include "cdtc/cpc_hardware_colors.s"

_show_stars::
        ;; ld bc,#0xbc0c
        ;; out (c),c
        ;; ld bc,#0xbd40
        ;; out (c),c

next_frame:
        ld l,#hardware_color_register_value_r0_g0_b0_black
        call _setborder
        call #0xbd19
        ld l,#hardware_color_register_value_r0_g0_b2_bright_blue
        call _setborder

        ld hl, #(_star_4pixelsperframe_pos_byte_l)

        ;; always 256 stars, else set l=(256-starcount)
        ;; c = mask
        ld c,#0x88
next_star4pix_:
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
        ld a,c
        ld (de),a

        ;; next star
        inc l

        jr nz, next_star4pix_
        ;djnz next_star4pix_

star1pix:
        ld l,#hardware_color_register_value_r0_g1_b0_green
        call _setborder

        ;; star1pix encode both address and mask

        ld hl, #(_star_1pixel_perframe_pos_byte_l)
        ;; always 256 stars, else set l=(256-starcount)
next_star1pix_:
        ;; read old address
        ld e,(hl)
        inc h
        ld d,(hl)

        ;; erase old star (crude, whole byte)
        xor a
        ld (de),a

        ;; get mask
        inc h
        ld a,(hl)

        ;; compute new mask
        rlca   ;    	1 nop
        jr nc, samebyte
        ld a,#0x10
        dec de
        set 6,d
samebyte:

        ;; de = new address
        ;; hl = table(mask)

        ;; draw new star
        ld (de),a

        ;; store new mask
        ld (hl),a
        dec h

        ;; de = new address
        ;; hl = table(high)

        ;; store new address
        ld (hl),d
        dec h
        ld (hl),e

        ;; next star
        inc l

        jr nz, next_star1pix_
        ;djnz next_star1pix_

        jr next_frame

        ;ret
