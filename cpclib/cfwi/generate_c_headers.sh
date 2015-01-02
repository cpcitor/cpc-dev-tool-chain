#!/bin/bash

if [[ "$#" == "0" ]]
then
        echo "WARNING: This is a very hackish script that breaks headers, waiting for you to repair them afterwards."
        echo "Do not run without knowing what you do. You have been warned."
        exit 1
fi


set -eu

sed <src/fw_nowrapperneeded.s -n 's/^._\(fw_[a-z_]*\) == 0x[0-9A-F][0-9A-F][0-9A-F][0-9A-F]$/\1/p' | \
        while read functionname
do
        packname="$(echo "$functionname" | sed -n 's/^\(fw_[^_]*\)_.*$/\1/p')"
        headername="include/cfwi/${packname}.h"

        prototype=$(echo "void $functionname(void);" )
        if
                fgrep "$prototype" "$headername"
        then
                echo "ok"
        else
                echo $prototype >> $headername
        fi
done

cd include/cfwi

{
        echo "#ifndef  __CFWI_H__"
        echo "#define __CFWI_H__"
        echo ""
        while read HEADER
        do
                echo "#include \"$HEADER\""
        done < <(ls -1b *.h | grep -v "^cfwi.h$")
        echo ""
        echo "#endif /* __CFWI_H__ */"
} >>cfwi.h.tmp
mv -vf cfwi.h.tmp cfwi.h
