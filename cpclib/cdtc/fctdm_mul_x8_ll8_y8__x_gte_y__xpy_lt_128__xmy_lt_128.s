.module _fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128

.globl _fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128_table

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

;_fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128_table == 0x3F00

_fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128_fill_table::
        ld de,#(_fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128_table)
                                ; e = 0 now because table is aligned
                                ; e will also be the current index. Neat!
        ld h,e                  ; hl = current_square (16bit)
        ld l,e
        ;; bc is a scratch variable
1$:
        ld c,h
        ld a,l                  ; ca = current_square

        srl c
        rr a
        srl c
        rr a

        ;; Thus, a = LSB(current_square >>2).

        ld (de), a              ; write to table
        inc d
        ld a,c
        ld (de), a
        dec d

        ;; Compute next square
        ;; Basically, that's hl = hl + index + index + 1
        ld b,#0
        ld c,e                  ; remember, e is also index
        scf
        adc hl,bc               ; we add index +1
        ; carry cleared already because no overflow so adc clears carry
        adc hl,bc               ; we add index

        inc de                  ; move pointer to next entry, increment index
        ld a,e
        and a
        jr nz,1$
        ret


_fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128::
        ;; assumes h > l > 0
        ld a,h
        sub l
        jp nc, numbers_in_order
        neg
numbers_in_order:
	ld d,#(>_fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128_table)
        ld e,a                  ; e=h0-l0
        ex de,hl
        ld c,(hl)
        inc h                   ; switch to MSB table
        ld b,(hl)
        ; bc=(h0-l0)^2

        ex de,hl
        ld a,h
        add l
        jp c, sum_of_operands_too_big

        ld e,a

        ex de,hl
        ld a,(hl)               ; read MSB
        dec h                   ; switch to LSB table
        ld l,(hl)               ; read LSB
        ld h,a

        ; hl=(h0+l0)^2

        ;; let's bet carry = 0. Why ? Because table is not over memory
	;; boundary so inc h set carry to zero.

        sbc hl,bc
        ret
numbers_not_in_order:
        ld hl,#(-1)
        ret
sum_of_operands_too_big:
        ld hl,#8
        ret
