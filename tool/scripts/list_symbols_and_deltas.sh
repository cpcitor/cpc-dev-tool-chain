#!/bin/bash

OLDADDRDEC=0
while read -u3 ADDR SYMBOL MODULE
do
    ADDRDEC=$( printf %d 0x$ADDR )
    if (( ADDRDEC < OLDADDRDEC ))
    then
        break
    fi
    if (( OLDADDRDEC > 0 ))
    then SIZE=$(( $ADDRDEC - $OLDADDRDEC ))
         echo -e "$OLDADDR $( printf %6d $SIZE ) \t $OLDMODULE \t $OLDSYMBOL"
    fi
    OLDADDRDEC=$ADDRDEC
    OLDADDR=$ADDR
    OLDMODULE=$MODULE
    OLDSYMBOL=$SYMBOL
done 3< <( grep -A 1000 ^_CODE ${1?please provide name of a .map file} | sed -n 's/^     0000\([0-9A-F][0-9A-F][0-9A-F][0-9A-F]\)  \([^ ]*\) *\([^ ]*\)$/\1 \2 \3/p' )
