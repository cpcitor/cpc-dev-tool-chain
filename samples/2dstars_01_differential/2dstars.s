        .module 2dstars_asm

        .z80

        .globl _star_4pixelsperframe_pos_byte_l
        .globl _star_1pixelsperframe_pos_byte_l
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
        ;; b = (star_count+1) mod 256
        ;; c = mask
        ld bc,#0x0088
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

        djnz next_star4pix_

star1pix:
        ld l,#hardware_color_register_value_r0_g1_b0_green
        call _setborder

        ;; star1pix encode both address and mask

        ld hl, #(_star_1pixelsperframe_pos_byte_l)
        ;; b = (star_count+1) mod 256
        ;; c = mask
        ld bc,#0x0088
next_star1pix_:
        ;; read old address
        ld e,(hl)
        inc h
        ld d,(hl)

        ;; erase old star (crude, whole byte)
        xor a
        ld (de),a

        ;; prepare new address computation
        ld b,a ; b=0	1 nop
        ld c,a ; c=0	1 nop

        ;; get mask
        inc h
        ld a,(hl)

        ;; compute new mask
        rlca   ;    	1 nop

        ;; compute new address
        rl c   ;    	2 nop
        ex de,hl ;    	1 nop
        sbc hl,bc;    	4 nop
        set 6,h

        ;; hl = new address
        ;; de = table(mask)

        ;; draw new star
        ld (hl),a

        ;; store new mask
        ld (de),a
        dec d

        ex de,hl

        ;; hl = table(high)
        ;; de = new address

        ;; store new address
        ld (hl),d
        dec h
        ld (hl),e

        ;; next star
        inc l

        djnz next_star1pix_

        jr next_frame

        ;ret
