	.area _CODE
.module getchar
_getchar::
;;_getchar_rr_s::
        call    0xBB18
        ld l,a
        xor a
        ld h,a
        ret

;; _getchar_rr_dbs::

;;         ld      a,e
;;         call    0xBB5A
;;         ret
