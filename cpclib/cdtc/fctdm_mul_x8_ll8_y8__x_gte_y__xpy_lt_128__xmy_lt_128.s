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
        ;; h*l
        ;; and returns the result in h l.
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

        ;; Input: h and l number to multiply.
        ;; It is assumed that 0 <     h+l  < 128.
        ;; It is assumed that 0 < abs(h-l) < 128.

        ;; Total time in typical case: 34 NOPs
_fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128::
        ;; assumes h > l > 0
        ld a,h                  ;	1
        sub l                   ;	1
        jp nc, numbers_in_order ;	if taken, 3, else 2
        neg                     ;	1
        ;; jp and neg combined make a total of 3 in both cases
numbers_in_order:
	ld d,#(>_fctdm_mul_x8_ll8_y8__x_gte_y__xpy_lt_128__xmy_lt_128_table) ;	1
        ld e,a                  ; e=h0-l0                                    ;	1
        ex de,hl                ;	1
        ld c,(hl)               ;	2
        inc h                   ; switch to MSB table           ;	     1
        ld b,(hl)               ;       2
        ; bc=(h0-l0)^2 >> 2

        ex de,hl                ;       1
        ld a,h                  ;       1
        add l                   ;       1
        jp c, sum_of_operands_too_big ; 3 (if not taken)

        ld e,a                  ;       1

        ex de,hl                ;       1
        ld a,(hl)               ; read MSB      ;       2
        dec h                   ; switch to LSB table   ;       1
        ld l,(hl)               ; read LSB      ;       2
        ld h,a                  ;       1

        ; hl=(h0+l0)^2 >> 2

        ;; carry = 0 here because of the "jp c" above. If carry was
	;; set we would not have taken this path, and instructions
	;; in-between (ex, ld, dec) do not change carry flag.

        sbc hl,bc               ;       4

        ;; hl = ( (h0+l0)^2 - (h0-l0)^2 ) >> 2 = h0*l0
        ret                     ;       3
sum_of_operands_too_big:
        ld hl,#8
        ret
