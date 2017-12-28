#!/bin/bash

function guess_keyboard_mapping()
{
    type -a xmodmap >/dev/null || { echo >&2 Cannot find xmodmap.  Cannot guess keyboard settings for caprice32. ; return 1 ; }

    QUERYKEYBOARD=" xmodmap -pke "
    #QUERYKEYBOARD=" cat spanish "
    
    read KEYCODE N24 EQUALS A1 A2 A3 A4 rest < <( $QUERYKEYBOARD | grep " 24 " )
    read KEYCODE N11 EQUALS T1 T2 T3 T4 rest < <( $QUERYKEYBOARD | grep " 11 " )

    FINGERPRINT="$A1$A2$A3$A4$T1$T2"

    case "$FINGERPRINT" in
	"qQaA2at")
	    echo keymap_us.map
	    return 0
	    ;;
	"aAqQeacute2")
	    echo keymap_fr_linux.map
	    return
	    ;;
	"qQqQ2at")
	    echo keymap_uk_linux.map
	    return
	    ;;
	"qQaA2quotedbl")
	    echo keymap_es_linux.map
	    return
	    ;;
	*)
	    echo >&2 "Please report to author with this information: $FINGERPRINT"
	    exit 1
	    ;;
    esac
    return 1
}

KEYMAPCODE=$(guess_keyboard_mapping)

if [[ -n "$KEYMAPCODE" ]]
then
    echo "Good! Guessed keyboard layout: $KEYMAPCODE"
    KEYMAPCODELINE="    -e s+kbd_layout=.*+kbd_layout=${KEYMAPCODE}+ "
else
    echo "WARNING: cannot detect keyboard layout, caprice32 will default to us."
    KEYMAPCODELINE=""
fi
    
sed -e "s|/usr/local/share/caprice32|$PWD/$1|" \
    -e "s|rom_path=.*|rom_path=$PWD/$1/rom|" \
    $KEYMAPCODELINE \
    < "$1/cap32.cfg" \
    > cap32_local.cfg
