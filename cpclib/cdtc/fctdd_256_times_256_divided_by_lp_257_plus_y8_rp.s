.module _fctdd_256_times_256_divided_by_lp_257_plus_y8_rp

.z80

        ;; Original implementation of a division routine.
        ;; Greetings to Euclides for paving the way! ;-p

_fctdd_256_times_256_divided_by_lp_257_plus_y8_rp_table == 0x3c00

_fctdd_256_times_256_divided_by_lp_257_plus_y8_rp_fill_table::
        ;; We create a table of 256 * 256 / (257 + y) for all unsigned bytes y.

        ;; First, 256 * 256 / 256 = 256.  Yeah, so what?
        
        ;; Let's focus on the value at some index k of our table.

        ;; It means we have q(k) (quotient) and r(k) (remainder) satisfying:
        ;; 256 * 256 = ( 257 + k ) * q(k) + r(k)  with  0 <= r(k) < 257+k.

        ;; Can we compute the q(k+1) qnd r(k+1) that will satisfy
        ;; 256 * 256 = ( 257 + (k+1) ) * q(k+1) + r(k+1)  with  0 <= r(k+1) < 257+k+1 ?

        ;; Let's try.  Start from what we know.
        ;; 256 * 256 = ( 257 + k ) * q(k) + r(k)  with  0 <= r(k) < 257+k.
        ;; to get closer, add and substract q(k)
        ;; 256 * 256 = ( 257 + k ) * q(k) + q(k) - q(k) + r(k)
        ;; factorize in front of q(k)
        ;; 256 * 256 = ( 257 + (k+1) ) * q(k) - q(k) + r(k)
        ;; arrange
        ;; 256 * 256 = ( 257 + (k+1) ) * q(k) + r(k) - q(k)

        ;; Since r(k) < 257+k+1, then r(k)-q(k) < 257+k+1.
        ;; Suffices that 0 <= r(k)-q(k) to we have our solution:
        ;; define
        ;; q(k+1) = q(k)
        ;; and
        ;; r(k+1) = r(k) - q(k)
        ;; then
        ;; 256 * 256 = ( 257 + (k+1) ) * q(k+1) + r(k+1)  with  0 <= r(k+1) < 257+k.

        ;; We don't yet cover all cases.  It may happen that r(k)-q(k)<0.
        ;; Then start again from what we know.
        ;; 256 * 256 = ( 257 + k ) * q(k) + r(k)  with  0 <= r(k) < 257+k.
        ;; to get closer, add and substract q(k)
        ;; 256 * 256 = ( 257 + k ) * q(k) + q(k) - q(k) + r(k)
        ;; factorize in front of q(k)
        ;; 256 * 256 = ( 257 + (k+1) ) * q(k) - q(k) + r(k)
        ;; to retain a positive remainder, we add and remove (257+(k+1))
        ;; 256 * 256 = ( 257 + (k+1) ) * (q(k) - 1) + (257+(k+1)) - q(k) + r(k)
        ;; let's try to sketch a solution.

        ;; define
        ;; q(k+1) = q(k)-1
        ;; and
        ;; r(k+1) = r(k) - q(k) + 257 + (k+1)
        ;; then
        ;; 256 * 256 = ( 257 + (k+1) ) * q(k+1) + r(k+1).
        ;; Remains to check whether 0 <= r(k+1) < 257+k.
        ;; We start from 0 <= r(k) < 257+k and r(k)-q(k)<0.


        ;; Because r(k)-q(k)<0,
        ;; by adding 257+(k+1) on both sides
        ;; r(k)-q(k) + 257+(k+1) < 257+(k+1)
        ;; so
        ;; r(k+1) < 257+(k+1).
        ;; We got one of the inequalities proven.
        ;; Now, remains to prove the other.

        ;; We removed q(k) from the previous remainder, and added 257+(k+1) to compensate.
        ;; It is enough that q(k) <= 257+(k+1) to get a positive remainder.
        ;; When does that happen?
        ;; Well, it happens that the quotient (q) is a decreasing function of the divisor (k).
        ;; and 257+k+1 is an increasing function of k.
        ;; So, if we find a k satisfying this condition, it will be true for all greater k.
        ;; Now, let's consider k=-1.
        ;; 256 * 256 = ( 257 + k ) * q(k) + r(k)  with  q(-1)=256 and r(-1)=0.
        ;; q(-1) = 256 <= 256 so this is true for k=-1.
        ;; Therefore it is true for all k>=-1.

        ;; Okay, so we have proven that we can compute a division table with a simple algorithm.

        ;; Let's explore the first iterations.
        ;; Each verifies
        ;; 256 * 256 = ( 257 + k ) * q(k) + r(k)  with  0 <= r(k) < 257+k.

        ;; 256 * 256 = 256 * 256 + 0	k = -1
        ;; 256 * 256 = 255 * 257 + 1	k = 0
        ;; 256 * 256 = 254 * 258 + 4	k = 1
        ;; 256 * 256 = 253 * 259 + 9	k = 2
        ;; 256 * 256 = 252 * 260 + 16	k = 3
        ;; 256 * 256 = 251 * 261 + 25	k = 4
        ;; 256 * 256 = 250 * 262 + 36	k = 5

        ;; Reserve a byte for value k, initial value 0.
        ;; Reserve a byte for value q.
        ;; Reserve a 10-bit value for r (it has to hold values up to 257+k with k up to 255, thus up to 257+255 = 512 possibly included).
        ;; We may argue that this can be reduced.  We'll check that later.

        ;; Algorithm:
        ;; start with k+1=0, q=0 (means q=256 here)
        ;; substract q from r           [10 bits - 8 bits]
        ;; on no carry, go to nocarry
        ;; decrement q                  [8 bits]
        ;; nocarry:
        ;; add to r, 256+k+1              [10 bits + 8 bits]
        ;; write q			[8 bits]
        ;; increment k+1			[8 bits]
        ;; loop if no carry

        ;; First iterations
        ;; 
        
        
        ;; Assign registers
        ;; hl = r
        ;; d = 1
        ;; e = k+1
        ;; bc = table
        ;; a = q

        ;; Assign registers
        ;; hl = r
        ;; d = 0
        ;; e = q
        ;; bc = table
        ;; a = k+1

        ;; Let's call "K"=k+1

        ;; Assign registers
        ;; hl = r
        ;; de = 256+K
        ;; bc = q
        ;; ix = table

        ;; xor a

        ;; ld h,a
        ;; ld l,a                  ; l = r, starts at 0

        ;; ld d,#1
        ;; ld e,a                  ; de = 256+K

        ;; ld b,a                  ; just to to SBC HL,DE
        ;; ld c,a                  ; bc = q
        ;; dec c                   ; q = 255
        ;; inc l                   ; r = 1

        ld ix,#(_fctdd_256_times_256_divided_by_lp_257_plus_y8_rp_table)

        ;; first iteration, we go from (unwritten) 256*256 = 256*256+0
        ;; to writing to table[0] that 256*256 = 255*257+1, so q=255 r=1

        ld hl,#1                ; r = 1
        ld de,#0x0100           ; 256 + K = 256 + 0
        ld bc,#0x00FF           ; q = 255

        jp nocarry

nextindex:
        sbc hl, bc              ; r = r - q
        jp NC, nocarry
        ;; benefit from carry set for ADC
        adc hl,de               ; r = r + 256 + K
        dec c                   ; in spirit this is 16 bit but 8 are enough, we know there's no need for carry
nocarry:
        ld 0(ix), c
        inc ix                  ; next element in table
        inc e
        jr nz, nextindex
        ret
