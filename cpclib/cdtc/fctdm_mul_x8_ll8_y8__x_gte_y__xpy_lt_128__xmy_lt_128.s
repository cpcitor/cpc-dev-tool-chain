.module _fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128

.z80

	;; This file implements a multiplication routine based on a table.
        ;;
        ;; The *_fill_table routine, as it names implies, fills a table.
        ;; It must be called once before running the multiplication routine.
        ;;
        ;; The multiplication routine assumes:
        ;;
        ;; 0 < l < h <=128
        ;; computes
        ;; h*l>>8
        ;; and returns the result in a and l.
        ;;

        ;; Inspired from https://www.msxcomputermagazine.nl/mccw/92/Multiplication/en.html
        ;; and adapted for my specific case of unsigned 8bit integers.

_fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128_table == 0x3F00

_fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128_fill_table::
        ld de,#(_fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128_table)
                                ; e = 0 now because table is aligned
                                ; e will also be the current index. Neat!
        ld h,e                  ; hl = current_square (16bit)
        ld l,e
        ;; bc is a scratch variable
1$:
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


_fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128::
        ;; assumes h > l > 0
        ld a,h
        sub l
        jp c, numbers_not_in_order

	ld d,#(>_fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128_table)
        ld e,a                  ; e=h0-l0
        ld a,(de)               ; b=(h0-l0)^2
        ld b,a

        ld a,h
        add l
        jp c, sum_of_operands_too_big

        ld e,a                  ; e=h0+l0
        ld a,(de)               ; a=(h0+l0)^2

        sub b
        ld l,a
        ret
numbers_not_in_order:
        ld l,#(-1)
        ret
sum_of_operands_too_big:
        ld l,#8
        ret
