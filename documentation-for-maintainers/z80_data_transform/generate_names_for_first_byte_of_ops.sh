#!/bin/bash

# John Adams a.k.a. https://github.com/borilla published
# https://borilla.co.uk/z80.html . Thanks for the data!

# This script written by Stéphane Gourichon (cpcitor) reads data
# imported from the above page and generates an assembly source file
# containing a symbol list suitable for use in the sdasz80 assembler
# targetting the Z80.

# The result is in
# cpc-dev-tool-chain/cpclib/cdtc/asminclude/cdtc/z80_syms_for_opcodes_first_byte.s

while read LINE
do
    if [[ "$LINE" =~ ^(.*)$'\t'.*$'\t'([0-9a-f][0-9a-f])\ ?([a-z 0-9]*)$ ]]
    then
        OP=${BASH_REMATCH[1]}
        OPCODEFIRSTBYTE=${BASH_REMATCH[2]}

        [[ "$OPCODEFIRSTBYTE" =~ ^(dd|ed|fd|cb) ]] && continue

        SYMBOLNAME=$(echo ${OP} | sed -E 's/[^a-z0-9]+/_/g' | sed -E 's/_*$//g')
        echo "z80_opcode_${SYMBOLNAME} .equ 0x$OPCODEFIRSTBYTE"
        echo "z80_opcode_${SYMBOLNAME}_0x$OPCODEFIRSTBYTE .equ 0x$OPCODEFIRSTBYTE"
    else
        echo "BAD $LINE"
    fi
done < <( grep '^\*' z80_Instruction_Set.txt | sed 's/\*//g' ) | column -t | sort
