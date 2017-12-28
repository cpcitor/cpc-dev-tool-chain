#!/bin/bash

set -eu

for ROM_URL in "${@}"
do
    ERRORS=""
    ROMFILENAME="$(basename "$ROM_URL")"
    echo
    if 
	unzip -lv "$ROMFILENAME" 2>/dev/null >/dev/null
    then
	echo "***** Skipping download already present and valid: $ROMFILENAME"
	echo
    else
	if ! wget -S ${ROM_URL} -O "${ROMFILENAME}.tmp"
	then
	    echo >&2 "WARNING: download failed ${ROM_URL}"
	    ERRORS+="${ROM_URL} "
	    continue
	fi

	if unzip -lv "$ROMFILENAME.tmp"
	then
	    mv -vf "${ROMFILENAME}.tmp" "${ROMFILENAME}"
	else
	    echo >&2 "WARNING: cannot confirm valid archive $ROMFILENAME.tmp"
	fi
    fi
done

if [[ -z "${ERRORS}" ]]
then
    exit 0
else
    echo >&2 "WARNING: download failed $ERRORS"
    exit 1
fi

