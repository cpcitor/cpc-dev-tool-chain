.module cdtc_sqscr_configure_crtc

        ;; This module depends on

.z80

;; Parameters. You can set these.


        .globl _crtc_r1_disp_width_chars
        .globl _crtc_r6_disp_height_chars
        .globl _cdtc_screen_basic__table_lh
        .globl _screen_first_byte_address
        .globl _screen_width_in_bytes

_cdtc_screen_basic__configure_crtc::
        ld bc, #0xbc01
        out (c), c
        ld bc, #0xbd00 + _crtc_r1_disp_width_chars
        out (c), c

        ;; horizontal centering
        ld bc, #0xbc02
        out (c), c
        ld bc, #0xbd00 + 46 + ((_crtc_r1_disp_width_chars-40)/2)
        out (c), c

        ld bc, #0xbc06
        out (c), c
        ld bc, #0xbd00 + _crtc_r6_disp_height_chars
        out (c), c

        ;; vertical centering
        ld bc, #0xbc07
        out (c), c
        ld bc, #0xbd00 + 30 + ((_crtc_r6_disp_height_chars-24)/2)
        out (c), c

        ld bc, #0xbc09
        out (c), c
        ld bc, #0xbd00 + (_crtc_r9_plus_1_disp_lines_per_char_height - 1)
        out (c), c

        ld bc, #0xbc09
        out (c), c
        ld bc, #0xbd00 + (_crtc_r9_plus_1_disp_lines_per_char_height - 1)
        out (c), c

        ret
