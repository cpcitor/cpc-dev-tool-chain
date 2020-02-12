#!/bin/bash

set -eu -o pipefail

rm -rf generated
mkdir -p generated/cells

dump()
{
    echo "t $TIMING page $PAGE op $OPC . Total \"$TIMING\" \"$BYTE1\" \"$BYTE2\" \"$BYTE3\" "
}

# trap 'dump' ERR

# Parse into table-like representation

while read TIMING BYTE1 BYTE2 BYTE3
do
    #echo -n .

    #echo ${BYTE1} ${TIMING}
    PAGE=1
    OPC=$BYTE1

    if [[ $BYTE1 == cb ]]
    then
        PAGE=2
        OPC=$BYTE2
    fi

    if [[ $BYTE1 == dd ]]
    then
        if [[ $BYTE2 == cb ]]
        then
            PAGE=6
            OPC=$BYTE3
        else
            PAGE=3
            OPC=$BYTE2
        fi
    fi

    if [[ $BYTE1 == ed ]]
    then
        PAGE=4
        OPC=$BYTE2
    fi

    if [[ $BYTE1 == fd ]]
    then
        if [[ $BYTE2 == cb ]]
        then
            PAGE=7
            OPC=$BYTE3
        else
            PAGE=5
            OPC=$BYTE2
        fi
    fi

    if [[ -z "$OPC" ]]
    then
        echo
        dump
    fi

    if [[ $TIMING =~ ([0-9]*)/([0-9]*) ]]
    then
        # echo "Warning : TIMING=$TIMING"
        TIMING=${BASH_REMATCH[2]}
        # echo "Warning : TIMING=$TIMING"
    fi

    printf '% 2u,' $TIMING >> generated/cells/table_${PAGE}_opcode_${OPC^^}.txt
done < <( grep -iv halt z80_Instruction_Set.txt | sed -E -n $'s|^.*\t([1-9]/?[0-9]* \t.*)|\\1|p' | sed 's/ nn//g' )

echo -n P2, >> generated/cells/table_1_opcode_CB.txt
echo -n P3, >> generated/cells/table_1_opcode_DD.txt
echo -n P4, >> generated/cells/table_1_opcode_ED.txt
echo -n P5, >> generated/cells/table_1_opcode_FD.txt
echo -n P6, >> generated/cells/table_3_opcode_CB.txt
echo -n P7, >> generated/cells/table_5_opcode_CB.txt

#

# static const char  z80pg1[256] = {
# /*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
# /*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */
# /*00*/   4,10, 7, 6, 4, 4, 7, 4, 4,11, 7, 6, 4, 4, 7, 4,
# /*10*/  13,10, 7, 6, 4, 4, 7, 4,12,11, 7, 6, 4, 4, 7, 4,
# /*20*/  12,10,16, 6, 4, 4, 7, 4,12,11,16, 6, 4, 4, 7, 4,
# /*30*/  12,10,13, 6,11,11,10, 4,12,11,13, 6, 4, 4, 7, 4,
# /*40*/   4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
# /*50*/   4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
# /*60*/   4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
# /*70*/   7, 7, 7, 7, 7, 7, 4, 7, 4, 4, 4, 4, 4, 4, 7, 4,
# /*80*/   4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
# /*90*/   4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
# /*A0*/   4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
# /*B0*/   4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
# /*C0*/  11,10,10,10,17,11, 7,11,11,10,10,P2,17,17, 7,11,
# /*D0*/  11,10,10,11,17,11, 7,11,11, 4,10,11,17,P3, 7,11,
# /*E0*/  11,10,10,19,17,11, 7,11,11, 4,10, 4,17,P4, 7,11,
# /*F0*/  11,10,10, 4,17,11, 7,11,11, 6,10, 4,17,P5, 7,11
# };

declare -a TITLES=(
    "unused"
    ""
"  /* P2 == CB */"
"  /* P3 == DD */"
"  /* P4 == ED */"
"  /* P5 == FD */"
"  /* P6 == DD CB */"
"  /* P7 == FD CB */"
)

{
    cat <<EOF

/*
 * Z80 Opcode Cycle Pages
 */
EOF

for PAGE in 1 2 3 4 5 6 7
do
    echo
    echo "static const char  z80pg${PAGE}[256] = {${TITLES[$PAGE]}"
    echo "/*--*--* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */"
    echo "/*--*--* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - */"

    for H in 0 1 2 3 4 5 6 7 8 9 A B C D E F
    do
        echo -n "/*${H}0*/  "
        for L in 0 1 2 3 4 5 6 7 8 9 A B C D E F
        do
            FILE=generated/cells/table_${PAGE}_opcode_${H}${L}.txt
            if [[ -r "$FILE" ]]
            then
                cat "$FILE"
            else
                echo -n "UN,"
            fi
        done
        echo
    done
    echo "};"
done
} > generated/tables_z80_cpc.c

#PATTERN1="static const char  z80pg1.256. = {"
PATTERN1="."
PATTERN2="static const char .z80Page.7. = {"

sed -n "/^${PATTERN1}/,/^${PATTERN2}/p;/^${PATTERN2}/q" < ${CDTC_ROOT}/tool/sdcc/sdcc-3.9.0/sdas/asz80/z80mch.c > generated/tables_sdasz80_default.c

diff -u generated/tables_sdasz80_default.c generated/tables_z80_cpc.c >| ../../tool/sdcc/z80_cpc_timings.diff || true

diffstat < ../../tool/sdcc/z80_cpc_timings.diff

echo "Now have to review and manually edit the patch to remove the first hunk."

echo "emacs ../../tool/sdcc/z80_cpc_timings.diff"
