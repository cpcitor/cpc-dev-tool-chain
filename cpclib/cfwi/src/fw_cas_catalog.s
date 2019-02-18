.module fw_cas_catalog

_fw_cas_catalog::
        ex      de,hl
	call	0xBC9B		; CAS CATALOG
        ld      h,a
        ld      a,#0
	jr	nz,nozeroflag
	inc	a               ; does not affect carry
nozeroflag:
        rlca                    ; propagates carry, does not change zero flag
	ld	h,a             ; thus HL gets: (errorbyte << 8 | carry | zero << 1)
	ret
