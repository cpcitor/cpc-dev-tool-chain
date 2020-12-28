.module fctdm

.z80

        ;; Inspired from https://www.msxcomputermagazine.nl/mccw/92/Multiplication/en.html
        ;; and adapted for my specific case of unsigned 8bit integers.

_fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_256__xmy_lt_256_table == 0x3D00

_fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_256__xmy_lt_256_fill_table::
        ld de,#(_fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_256__xmy_lt_256_table)
                                ; e = 0 now because table is aligned
                                ; e will also be the current index. Neat!
        ld h,e                  ; hl = current_square (16bit)
        ld l,e
        ;; bc is a scratch variable
1$:
        ;; TODO THIS IS WRONG. HAS TO COMPUTE SQUARES ON 24bit BECAUSE 511*511=776001=0x3FC01.
        ld a,h
        ld c,l                  ; ac = current_square
        and a                   ; clear carry
        ;; rlc c
        ;; rl a                    ; ac = current_square << 1
        ;; rlc c
        ;; rl a                    ; ac = current_square << 2
        ;; rlc c
        ;; rl a                    ; ac = current_square << 3
        ;; rlc c
        ;; rl a                    ; ac = current_square << 4
        ;; rlc c
        ;; rl a                    ; ac = current_square << 5
        ;; rlc c
        ;; rl a                    ; ac = current_square << 6
        ;; Thus, a = current_square >>2 .

        ld (de), a              ; write to table

        ;; Compute next square
        ;; Basically, that's hl = hl + index + index + 1
        ld b,#0
        ld c,e                  ; remember, e is also index
        scf
        adc hl,bc               ; we add index +1
        ; carry cleared already because no overflow and a                   ; clear carry
        adc hl,bc               ; we ass index

        inc de                  ; move pointer to next entry, increment index
        ld a,e
        and a
        jr nz,1$
        ret


_fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_256__xmy_lt_256::
        ;; assumes h > l > 0
        ld a,h
        sub l
        jp c, numbers_not_in_order

	ld d,#(>_fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_256__xmy_lt_256_table)
        ld e,a                  ; e=h0-l0
        ld a,(de)               ; b=(h0-l0)^2
        ld b,a

        ld a,h
        ld h,#0
        add l
        jp nc, first_half_of_table
        inc hl
first_half_of_table:
                                ; hl=h0+l0
        add hl,de

        ld a,(hl)               ; a=(h0+l0)^2
        sub b
        ld l,a
        ret
numbers_not_in_order:
        ld l,#(~0)
        ret
