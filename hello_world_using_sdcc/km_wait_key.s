	;; Call KM_WAIT_KEY firmware routine
	;; and put return value where C compiler expects it.

	.globl _km_wait_key

;	---------------------------------
; Function km_wait_key
; ---------------------------------
_km_wait_key:
        call    0xBB18
	;; return value in l register
	ld	l,a
	ret
