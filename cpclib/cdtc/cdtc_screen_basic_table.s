.module cdtc_sqscr_scraddrtable_fill

.z80

        .globl _crtc_r1_disp_width_chars
        .globl _crtc_r6_disp_height_chars
        .globl _crtc_r9_plus_1_disp_lines_per_char_height
        .globl _cdtc_screen_basic__table_hl
        .globl _screen_first_byte_address
        .globl _screen_width_in_bytes

        .macro with_ix INSTRUCTION
        .byte 0xdd
        INSTRUCTION
        .endm

        .macro with_iy INSTRUCTION
        .byte 0xfd
        INSTRUCTION
        .endm


;;_cdtc_screen_basic__table_hl == 0x5A00

_cdtc_screen_basic__table_hl__fill::
	;; Now we're interested in a _crtc_r1_disp_width_chars * _crtc_r6_disp_height_chars screen.
	;; Each line is 64 byte wide.
	;; 0xC000
	;; 0xC800 + 0x800 always
	;; 0xD000
	;; 0xD800
	;; 0xE000
	;; 0xE800
	;; 0xF000
	;; 0xF800
	;; 0xC020 (why ? Because 0x0000 -> overflow -> go back 0x4000 then add _screen_width_in_bytes -> 0xC020). Hmfhmf no. Because we exhausted _crtc_r9_plus_1_disp_lines_per_char_height. This way is more general.
        ;; 0xC820
        ;; 0xD020
        ;; 0xD820
	;; Loop until exhausted _screen_total_lines.

	ld iy, #_cdtc_screen_basic__table_hl
        ld hl, #_screen_first_byte_address
        ld de, #_screen_width_in_bytes
        ;; b used to count scanline lines in characters
        ld c, #_crtc_r6_disp_height_chars

	;; now loop
next_char_big_line:
        ld b, #_crtc_r9_plus_1_disp_lines_per_char_height

next_scan_line_in_char:
	;; write value in table
        ld 0(iy), h   ;; first write high byte
        with_iy ^/inc h/ ;; go to second table
        ld 0(iy), l   ;; there, write low byte
        with_iy ^/dec h/ ;; go back to first table
        with_iy ^/inc l/ ;; next entry in table

	ld a, #8
	add a, h
	ld h, a
        djnz next_scan_line_in_char

        ;; Ok we've done one "character" (or possibly less than 7, anyway).
        ;; Now we have to jump to next line.

        ;; "a" still contains high byte of address (hl)
	;; we have to go back 0x4000
	sub #0x40               ; probably could just "and" as well ? Anyway...

	;; then add alternate (has to be 16 bit add)
        ld h, a
        add hl, de

        dec c
        jr nz, next_char_big_line

        ret

        ;; Example code showing how to
        ;; get screen address at run time.
        ;; ld h, # (> _cdtc_screen_basic__table_hl )
        ;; ld l, #0
        ;; ld c, (hl)
        ;; inc h
        ;; ld b, (hl)

        ;; input : HL = current address
        ;; output: HL = new address
        ;; corrupts: A
_cdtc_screen_basic__previous_line::

        ;; original

        ;; ld a,h                  ; T:1
        ;; sub #08                 ; T:2
        ;; ld h,a                  ; T:1

        ;; ;; if the line is not
        ;; and #38                 ; T:2
        ;; cp #38                  ; T:2
        ;; ret nz                  ; T:4 if returning, else 2

        ;; Total : 12 if returning


        ;; better ? not

        ;; ld a, 0x38              ; T:2
        ;; and h                   ; T:1
        ;; jr z, notsimplest       ; T:2 if returning, else 3
        ;; ld a,h                  ; T:1
        ;; sub #08                 ; T:2
        ;; ld h,a                  ; T:1
        ;; ret                     ; T:3

        ;; Total : 12 if returning


        ;; try again

        ;; simplest case, covers 7 times out of 8 or 224/256.  Pretty fast actually.
        ld a,h                  ; T:1
        sub #08                 ; T:2
        ld h,a                  ; T:1

        ;; Are we in a simple case?
        and #38                 ; T:2
        cp #38                  ; T:2
        ret nz                  ; T:4 if returning, else 2

        ;; Total : 12 if returning.

        ;; Okay, we got too far. Add 0x4000 to get back into screen bank.
        ld a,h
        add #40
        ld h,a

        ;; Now we know that one line above means moving R1*2 back,
	;; let's do it.
        ld a,l
        sub #_screen_width_in_bytes
        ld l,a
        ;; If 8-bit operation was enough, nothing more to do.
        ret nc

        ;; Perhaps some values of R1 (like 32) means we never go here.
        ;; Anyway.

        ;; 8 bit was not enough.
        ld a,h
        dec h
        and #07
        ret nz

        ld a,h
        add #08
        ld h,a
        ret
