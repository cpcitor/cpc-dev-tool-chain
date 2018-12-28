	.module setborder

_setborder::
	ld	bc,#0x7F10
	out	(c),c
	out	(c),l
	ret
