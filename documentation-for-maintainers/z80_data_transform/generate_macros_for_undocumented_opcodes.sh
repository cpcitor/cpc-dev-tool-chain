#!/bin/bash

# John Adams a.k.a. https://github.com/borilla published
# https://borilla.co.uk/z80.html . Thanks for the data!

# This script written by Stéphane Gourichon (cpcitor) reads data
# imported from the above page and generates an assembly source file
# containing macros suitable for use in the sdasz80 assembler
# targetting the Z80.

# The result is in
# cpc-dev-tool-chain/cpclib/cdtc/asminclude/cdtc/z80_undocumented_opcodes.macros.s

while read OP OPAND TIMING PREFIX OPCODE
do
    MACRONAME=$(echo ${OP}__${OPAND} | sed 's/[ ,]/_/g')
    ARG=""
    ADDBYTES=""
    if [[ $OPAND =~ nn ]]
    then
         ARG=" nn"
    fi
    echo ";; ${OP} ${OPAND}.  Duration: $TIMING NOPs"
    echo ".macro ${MACRONAME}${ARG}"
    echo ".db 0x${PREFIX}, 0x${OPCODE/ nn/, nn}"
    echo .endm
    echo
done < <( grep i[xy][lh] z80_Instruction_Set.txt  | sed 's/\*//g' )
