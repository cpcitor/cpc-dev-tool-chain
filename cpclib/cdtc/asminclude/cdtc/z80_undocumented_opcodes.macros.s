;; This file was generated with the help of a bash script from data published at https://borilla.co.uk/z80.html . Thanks John Adams a.k.a. https://github.com/borilla !
;; It is intended to be used like this:
;; .include "z80_undocumented_opcodes.s"
;; ...your code...
;; adc__a_ixh

;; The script is in
;; cpc-dev-tool-chain/documentation-for-maintainers/z80_data_transform/generate_macros_for_undocumented_opcodes.sh

;; adc a,ixh.  Duration: 2 NOPs
.macro adc__a_ixh
.db 0xdd, 0x8c
.endm

;; adc a,iyh.  Duration: 2 NOPs
.macro adc__a_iyh
.db 0xfd, 0x8c
.endm

;; adc a,ixl.  Duration: 2 NOPs
.macro adc__a_ixl
.db 0xdd, 0x8d
.endm

;; adc a,iyl.  Duration: 2 NOPs
.macro adc__a_iyl
.db 0xfd, 0x8d
.endm

;; add a,ixh.  Duration: 2 NOPs
.macro add__a_ixh
.db 0xdd, 0x84
.endm

;; add a,iyh.  Duration: 2 NOPs
.macro add__a_iyh
.db 0xfd, 0x84
.endm

;; add a,ixl.  Duration: 2 NOPs
.macro add__a_ixl
.db 0xdd, 0x85
.endm

;; add a,iyl.  Duration: 2 NOPs
.macro add__a_iyl
.db 0xfd, 0x85
.endm

;; and ixh.  Duration: 2 NOPs
.macro and__ixh
.db 0xdd, 0xa4
.endm

;; and iyh.  Duration: 2 NOPs
.macro and__iyh
.db 0xfd, 0xa4
.endm

;; and ixl.  Duration: 2 NOPs
.macro and__ixl
.db 0xdd, 0xa5
.endm

;; and iyl.  Duration: 2 NOPs
.macro and__iyl
.db 0xfd, 0xa5
.endm

;; dec ixh.  Duration: 2 NOPs
.macro dec__ixh
.db 0xdd, 0x25
.endm

;; dec iyh.  Duration: 2 NOPs
.macro dec__iyh
.db 0xfd, 0x25
.endm

;; dec ixl.  Duration: 2 NOPs
.macro dec__ixl
.db 0xdd, 0x2d
.endm

;; dec iyl.  Duration: 2 NOPs
.macro dec__iyl
.db 0xfd, 0x2d
.endm

;; inc ixh.  Duration: 2 NOPs
.macro inc__ixh
.db 0xdd, 0x24
.endm

;; inc iyh.  Duration: 2 NOPs
.macro inc__iyh
.db 0xfd, 0x24
.endm

;; inc ixl.  Duration: 2 NOPs
.macro inc__ixl
.db 0xdd, 0x2c
.endm

;; inc iyl.  Duration: 2 NOPs
.macro inc__iyl
.db 0xfd, 0x2c
.endm

;; ld a,ixh.  Duration: 2 NOPs
.macro ld__a_ixh
.db 0xdd, 0x7c
.endm

;; ld a,iyh.  Duration: 2 NOPs
.macro ld__a_iyh
.db 0xfd, 0x7c
.endm

;; ld a,ixl.  Duration: 2 NOPs
.macro ld__a_ixl
.db 0xdd, 0x7d
.endm

;; ld a,iyl.  Duration: 2 NOPs
.macro ld__a_iyl
.db 0xfd, 0x7d
.endm

;; ld b,ixh.  Duration: 2 NOPs
.macro ld__b_ixh
.db 0xdd, 0x44
.endm

;; ld b,iyh.  Duration: 2 NOPs
.macro ld__b_iyh
.db 0xfd, 0x44
.endm

;; ld b,ixl.  Duration: 2 NOPs
.macro ld__b_ixl
.db 0xdd, 0x45
.endm

;; ld b,iyl.  Duration: 2 NOPs
.macro ld__b_iyl
.db 0xfd, 0x45
.endm

;; ld c,ixh.  Duration: 2 NOPs
.macro ld__c_ixh
.db 0xdd, 0x4c
.endm

;; ld c,iyh.  Duration: 2 NOPs
.macro ld__c_iyh
.db 0xfd, 0x4c
.endm

;; ld c,ixl.  Duration: 2 NOPs
.macro ld__c_ixl
.db 0xdd, 0x4d
.endm

;; ld c,iyl.  Duration: 2 NOPs
.macro ld__c_iyl
.db 0xfd, 0x4d
.endm

;; ld d,ixh.  Duration: 2 NOPs
.macro ld__d_ixh
.db 0xdd, 0x54
.endm

;; ld d,iyh.  Duration: 2 NOPs
.macro ld__d_iyh
.db 0xfd, 0x54
.endm

;; ld d,ixl.  Duration: 2 NOPs
.macro ld__d_ixl
.db 0xdd, 0x55
.endm

;; ld d,iyl.  Duration: 2 NOPs
.macro ld__d_iyl
.db 0xfd, 0x55
.endm

;; ld e,ixh.  Duration: 2 NOPs
.macro ld__e_ixh
.db 0xdd, 0x5c
.endm

;; ld e,iyh.  Duration: 2 NOPs
.macro ld__e_iyh
.db 0xfd, 0x5c
.endm

;; ld e,ixl.  Duration: 2 NOPs
.macro ld__e_ixl
.db 0xdd, 0x5d
.endm

;; ld e,iyl.  Duration: 2 NOPs
.macro ld__e_iyl
.db 0xfd, 0x5d
.endm

;; ld ixh,a.  Duration: 2 NOPs
.macro ld__ixh_a
.db 0xdd, 0x67
.endm

;; ld ixh,b.  Duration: 2 NOPs
.macro ld__ixh_b
.db 0xdd, 0x60
.endm

;; ld ixh,c.  Duration: 2 NOPs
.macro ld__ixh_c
.db 0xdd, 0x61
.endm

;; ld ixh,d.  Duration: 2 NOPs
.macro ld__ixh_d
.db 0xdd, 0x62
.endm

;; ld ixh,e.  Duration: 2 NOPs
.macro ld__ixh_e
.db 0xdd, 0x63
.endm

;; ld ixh,nn.  Duration: 3 NOPs
.macro ld__ixh_nn nn
.db 0xdd, 0x26, nn
.endm

;; ld iyh,a.  Duration: 2 NOPs
.macro ld__iyh_a
.db 0xfd, 0x67
.endm

;; ld iyh,b.  Duration: 2 NOPs
.macro ld__iyh_b
.db 0xfd, 0x60
.endm

;; ld iyh,c.  Duration: 2 NOPs
.macro ld__iyh_c
.db 0xfd, 0x61
.endm

;; ld iyh,d.  Duration: 2 NOPs
.macro ld__iyh_d
.db 0xfd, 0x62
.endm

;; ld iyh,e.  Duration: 2 NOPs
.macro ld__iyh_e
.db 0xfd, 0x63
.endm

;; ld iyh,nn.  Duration: 3 NOPs
.macro ld__iyh_nn nn
.db 0xfd, 0x26, nn
.endm

;; ld ixl,a.  Duration: 2 NOPs
.macro ld__ixl_a
.db 0xdd, 0x6f
.endm

;; ld ixl,b.  Duration: 2 NOPs
.macro ld__ixl_b
.db 0xdd, 0x68
.endm

;; ld ixl,c.  Duration: 2 NOPs
.macro ld__ixl_c
.db 0xdd, 0x69
.endm

;; ld ixl,d.  Duration: 2 NOPs
.macro ld__ixl_d
.db 0xdd, 0x6a
.endm

;; ld ixl,e.  Duration: 2 NOPs
.macro ld__ixl_e
.db 0xdd, 0x6b
.endm

;; ld ixl,nn.  Duration: 3 NOPs
.macro ld__ixl_nn nn
.db 0xdd, 0x2e, nn
.endm

;; ld iyl,a.  Duration: 2 NOPs
.macro ld__iyl_a
.db 0xfd, 0x6f
.endm

;; ld iyl,b.  Duration: 2 NOPs
.macro ld__iyl_b
.db 0xfd, 0x68
.endm

;; ld iyl,c.  Duration: 2 NOPs
.macro ld__iyl_c
.db 0xfd, 0x69
.endm

;; ld iyl,d.  Duration: 2 NOPs
.macro ld__iyl_d
.db 0xfd, 0x6a
.endm

;; ld iyl,e.  Duration: 2 NOPs
.macro ld__iyl_e
.db 0xfd, 0x6b
.endm

;; ld iyl,nn.  Duration: 3 NOPs
.macro ld__iyl_nn nn
.db 0xfd, 0x2e, nn
.endm

;; or ixh.  Duration: 2 NOPs
.macro or__ixh
.db 0xdd, 0xb4
.endm

;; or iyh.  Duration: 2 NOPs
.macro or__iyh
.db 0xfd, 0xb4
.endm

;; or ixl.  Duration: 2 NOPs
.macro or__ixl
.db 0xdd, 0xb5
.endm

;; or iyl.  Duration: 2 NOPs
.macro or__iyl
.db 0xfd, 0xb5
.endm

;; sbc a,ixh.  Duration: 2 NOPs
.macro sbc__a_ixh
.db 0xdd, 0x9c
.endm

;; sbc a,iyh.  Duration: 2 NOPs
.macro sbc__a_iyh
.db 0xfd, 0x9c
.endm

;; sbc a,ixl.  Duration: 2 NOPs
.macro sbc__a_ixl
.db 0xdd, 0x9d
.endm

;; sbc a,iyl.  Duration: 2 NOPs
.macro sbc__a_iyl
.db 0xfd, 0x9d
.endm

;; sub ixh.  Duration: 2 NOPs
.macro sub__ixh
.db 0xdd, 0x94
.endm

;; sub iyh.  Duration: 2 NOPs
.macro sub__iyh
.db 0xfd, 0x94
.endm

;; sub ixl.  Duration: 2 NOPs
.macro sub__ixl
.db 0xdd, 0x95
.endm

;; sub iyl.  Duration: 2 NOPs
.macro sub__iyl
.db 0xfd, 0x95
.endm

;; xor ixh.  Duration: 2 NOPs
.macro xor__ixh
.db 0xdd, 0xac
.endm

;; xor iyh.  Duration: 2 NOPs
.macro xor__iyh
.db 0xfd, 0xac
.endm

;; xor ixl.  Duration: 2 NOPs
.macro xor__ixl
.db 0xdd, 0xad
.endm

;; xor iyl.  Duration: 2 NOPs
.macro xor__iyl
.db 0xfd, 0xad
.endm
