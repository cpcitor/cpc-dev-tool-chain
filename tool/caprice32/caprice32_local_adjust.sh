#!/bin/bash

sed -e "s|/usr/local/share/caprice32|$PWD/$1|" \
    -e "s|rom_path=.*|rom_path=$PWD/$1/rom|" \
    < "$1/cap32.cfg" \
    > cap32_local.cfg

