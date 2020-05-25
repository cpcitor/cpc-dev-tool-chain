	;; Plug a putchar implementation suitable for the CPC.
	;; From http://www.cpcmania.com/Docs/Programming/Introduction_to_programming_in_SDCC_Compiling_and_testing_a_Hello_World.htm

	;; FILE: putchar.s
	;; Modified to suit execution on the Amstrad CPC
	;; by H. Hansen 2003
	;; Original lines has been marked out!

.module putchar
	.area _CODE
_putchar::
_putchar_rr_s::
        ld      hl,#2
        add     hl,sp

        ld      a,(hl)
        call    0xBB5A
        ret

_putchar_rr_dbs::

        ld      a,e
        call    0xBB5A
        ret
