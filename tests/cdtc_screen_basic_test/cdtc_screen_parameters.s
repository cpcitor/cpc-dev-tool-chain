; You can customize values below, to your wishes and context

_cdtc_screen_basic__table_lh == 0x100 ; if you don't return to BASIC, you can overwrite 0x100-0x2FF
_screen_first_byte_address == 0xC000
_crtc_r1_disp_width_chars == 32    ; 32 chars, 64 bytes, 256-pixel wide screen (standard is 40)
_crtc_r6_disp_height_chars == 32   ; 32 chars, 256-pixel high screen
_crtc_r9_plus_1_disp_lines_per_char_height == 8 ; standard value 8, r9 == 7

; Don't change lines below

_screen_width_in_bytes == _crtc_r1_disp_width_chars * 2
_screen_total_lines == _crtc_r6_disp_height_chars * _crtc_r9_plus_1_disp_lines_per_char_height



;; Some sanity checks

.iif gt, (_crtc_r1_disp_width_chars - 63) .mexit ; ERROR CPC only accepts up to 63 characters wide screen

.iif gt, (_crtc_r6_disp_height_chars - 32) .mexit ; ERROR This routine so far cannot handle screens with more than 256 lines.  Please comment, vote or contribue on https://github.com/cpcitor/cpc-dev-tool-chain/issues

.iif gt, (_crtc_r1_disp_width_chars * _crtc_r6_disp_height_chars - 1024) .mexit ; ERROR screen specifications too big.  CPC cannot output more that 1024 characters in MODE 1 without overscan.  This routine does not support 32k overscan screens yet.
