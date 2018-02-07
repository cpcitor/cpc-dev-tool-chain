	;; crt0.s - A crt0 in Z80 assembler language targeting Amstrad CPC

	;; Copyright (C) 2013 Stéphane Gourichon / cpcitor

	;  This library is free software; you can redistribute it and/or modify it
	;  under the terms of the GNU General Public License as published by the
	;  Free Software Foundation; either version 2, or (at your option) any
	;  later version.
	;
	;  This library is distributed in the hope that it will be useful,
	;  but WITHOUT ANY WARRANTY; without even the implied warranty of
	;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	;  GNU General Public License for more details.
	;
	;  You should have received a copy of the GNU General Public License
	;  along with this library; see the file COPYING. If not, write to the
	;  Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
	;   MA 02110-1301, USA.
	;
	;  As a special exception, if you link this library with other files,
	;  some of which are compiled with SDCC, to produce an executable,
	;  this library does not by itself cause the resulting executable to
	;  be covered by the GNU General Public License. This exception does
	;  not however invalidate any other reasons why the executable file
	;   might be covered by the GNU General Public License.
	;--------------------------------------------------------------------------

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;; Since this will be used by CPC coders, which are usually
	;; not savvy of C compiling/linking/init internals, this file
	;; is abundantly commented.
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	;; The module name appears in many compiler/linker log files,
	;; so it's important to define it.

	.module crt0


	;; We will reference the C-level symbol "main".
	;; The line below is equivalent to C-level "extern void main();"

	.globl _main



	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;; Do we need an absolutely positioned HEADER area ?
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	;; Short answer: no for a RAM program.

	;; Most z80 crt0 start with dedicating 0x38 bytes to interrupt
        ;; vectors. This makes sense only when making an image that
        ;; starts at address 0, which is not the case for a CPC RAM
        ;; program or upper ROM program.
	;; This crt0 targets a RAM program. So nothing to do at this step.


	;; Creating absolutely positioned linker areas would just put
	;; constraints and yield more maintenance work.

	;; We can avoid that and just let the linker pack areas one
	;; after the other.  So, no ".org 0x" here. The simplest thing
	;; to do with SDCC's Z80 target is to set location at only one
	;; place : the --code-loc option of sdcc.

	;; Hence, we start with the CODE area to ensure we start at
	;; the requested location.

	.area	_CODE

cpc_run_address::
init:
        ;; Initialise global variables, see below.
        call    gsinit
	jp	_main

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;; Do we need an _exit symbol ?
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	;; Short answer: not confirmed, so not done.

	;; SDCC's default crt0 defines a _exit symbol that does "ld a,#0 ; rst 0x08".
	;; This is supposed to allow the C-level exit() function to do what people expect.
	;; This won't work on the CPC.

	;; We have three choices :

	;; - just don't implement exit()

	;; - implement it with a "ret". This would enable calling
	;; "exit(value);" form main(). It has limited interest because
	;; "return value;" already works (FIXME check which register
	;; holds return value). Unfortunately, from another C function
	;; it would just return from the current function, not exit
	;; the program. So, not really useful.

	;; - implement it with "rst 0x00". This should reset the CPC.

	;; - record stack pointer before calling _main, put it back on
	;; _exit, set the return value in register and "ret". That
	;; would work if the C code has not killed the firmware RAM
	;; area.

	;; That last option would be useful especially when using
	;; cpc-dev-tool-chain to implement some RSX extensions that
	;; need to call exit().

	;; Until the need is confirmed we do nothing.



	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;; Initialize global variables.
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	;; Why must we care ? Else global variables are not
        ;; initialized.

	;; How this happens ?

	;; Compiler does not assume that compiled output can be a RAM.

	;; Indeed, if all compiled data lands in a ROM so do
 	;; initialization values. Then we must copy them to RAM.

	;; What SDCC does: dedicate an area named _INITIALIZED to
	;; run-time access to initialized global variables.  This
	;; assumes we instruct the linker to put such an area
	;; somewhere in RAM.

	;; Initial values of initialized global variables are provided
	;; in another region named _INITIALIZER.


	;; * "ROM program" case

	;; This makes total sense if linker output lands in a ROM. The
	;; only option is to copy _INITIALIZER to _INITIALIZED (modulo
	;; some possible compression tricks).


	;; * "RAM program" case

	;; If linker output already lands in RAM, as in a CPC "RAM
        ;; program", this works also but wastes an amount of RAM equal
        ;; to the amount of initialized data.

	;; We may write an external script to trick the linker to
	;; allocate both area in an absolute fashion to the same
	;; address. No wasted bytes, no copy.

        ;; One might think: why bother, I'll just won't use
        ;; initialized global variables syntax at C level, and
        ;; initialize my variables in C function code.  This works but
        ;; (1) makes code use even more bytes (2) forces poor style at
        ;; C source level.

	;; One might think: let's just use function-local C variables.
        ;; This is elegant in source code, but worse in generated
        ;; assembly code size and performance because function-local C
        ;; variables are accessed through the stack which is slower
        ;; because of extra indirection level.

	;; Conclusion

	;; So far we do simple and waste some bytes, that's ok.

	;; If/when need is confirmed, the trick to absolutely position
	;; both area at same position may be used.  Or tell the
	;; compiled that code is in RAM ? FIXME Write that to
	;; sdcc-devel mailing-list.

	.area   _GSINIT

	.globl l__INITIALIZER
	.globl s__INITIALIZED
	.globl s__INITIALIZER

gsinit::
	ld	bc, #l__INITIALIZER ;; We'll copy that many bytes.
	ld	a, b
	or	a, c
	jr	Z, gsinit_next      ;; If nothing to copy, don't
	ld	de, #s__INITIALIZED ;; set destination address
	ld	hl, #s__INITIALIZER ;; set source address
	ldir
gsinit_next:

	.area   _GSFINAL
	ret

	.area   _DATA
	.area 	_HOME
	.area   _INITIALIZER
	.area   _INITIALIZED
	.area	_AFTERCODE
_aftercode::
