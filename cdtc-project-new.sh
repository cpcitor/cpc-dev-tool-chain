#!/bin/bash

set -eu

usage() {
        echo "Take a directory full of source code and make a reference to cpc-dev-tool-chain so that it can build it."
        echo
        echo "Usage: $0 (directoryname)*"
        echo "Notice: directoryname can be just '.' to mean 'current directory'."
}

if [ "$#" == "0" ]
then
        usage
        exit 1
fi

cd "$(dirname "$(readlink -f "$0")" )"
SCRIPTDIR="$PWD"
cd - >/dev/null

CDTC_ROOT="${SCRIPTDIR}"

OVERWRITABLE_MARKER="THIS_FILE_WILL_BE_OVERWRITTEN_BY_CDTC"

proceed_with_one_item ()
{
        set -eu
        echo "Current dir is : $PWD"
        echo "Proceeding with : $1"
        cd "$1" || {
                echo "Not a directory I can go into : $1"
                echo "Trying to create a new directory."
                mkdir -p "$1" || { echo "Could not create directory. Aborting." ; return 1; }
                cd "$1" || { echo "Could not enter newly created directory. Aborting." ; return 1; }
        }

        if [[ -r Makefile ]]
        then
                echo "There's already a makefile in: $1"
                if grep -q "$OVERWRITABLE_MARKER" Makefile
                then
                        echo "Notice: will overwrite old generated Makefile in: $1"
                else
                        echo "There's already a makefile that I should probably not overwrite in: $1"
                        return 1
                fi
        fi

        {
                echo "# ${OVERWRITABLE_MARKER}"
                echo "-include cdtc_project.conf"
                echo "-include cdtc_local_machine.conf"
                echo "-include \$(CDTC_ROOT)/sdcc-project.Makefile"
                echo "failure:"
                echo -e "\011@echo 'Cannot locate cpc-dev-tool-chain main directory.'"
                echo -e "\011@echo 'Hint: edit (or create) cdtc_local_machine.conf and adjust line CDTC_ROOT=/path/to/cpc-dev-tool-chain'"
                echo -e "\011@echo 'If you don't have a clue: this project appears to rely on cpc-dev-tool-chain for compilation.'"
                echo -e "\011@echo 'See https://github.com/cpcitor/cpc-dev-tool-chain for instructions.'"
                echo -e "\011@false"
        } >Makefile.tmp

        mv -f Makefile.tmp Makefile

        PROJNAME="$(basename "$PWD" | sed 's/[^a-zA-Z0-9]*//g' | tr 'A-Z' 'a-z' | sed 's/^\(........\).*$/\1/' )"

        if ! [[ -e cdtc_project.conf ]]
        then
                echo "Generating new cdtc_project.conf"
                {
                        echo "# This file in Makefile syntax is intended to project-related variables"
                        echo "# like project name, compilation flags and the like."
                        echo "# If you use version control (you should anyway), "
                        echo "# include this file in your versioned project history."
                        echo "PROJNAME=$PROJNAME"
                } >cdtc_project.conf.tmp
                echo "Auto-generated PROJNAME=$PROJNAME"
                mv -f cdtc_project.conf.tmp cdtc_project.conf
        else
                echo "cdtc_project.conf exists already. Not setting PROJNAME etc in it."
        fi

        if grep -q "^CDTC_ROOT" cdtc_project.conf
        then
                grep -v "^CDTC_ROOT" cdtc_project.conf >cdtc_project.conf.tmp && mv -vf cdtc_project.conf.tmp cdtc_project.conf
        fi

        echo "Setting in cdtc_local_machine.conf CDTC_ROOT=${CDTC_ROOT}"
        {
                if [[ -e cdtc_local_machine.conf ]]
                then
                        grep -v "^CDTC_ROOT" cdtc_local_machine.conf
                else
                        echo "# This file in Makefile syntax is intended to contain only variables"
                        echo "# that are specific to this machine.  Do not distribute this file or put it"
                        echo "# in version control, as its content will not be relevant on other machines."
                        echo "# and would cause errors."
                fi
                echo "CDTC_ROOT=${CDTC_ROOT}"
        } >cdtc_local_machine.conf.tmp

        mv -f cdtc_local_machine.conf.tmp cdtc_local_machine.conf

        return 0
}

while [ "$#" -gt 0 ]
do
        proceed_with_one_item "$1" || echo "Failed for $1"
        shift
done
