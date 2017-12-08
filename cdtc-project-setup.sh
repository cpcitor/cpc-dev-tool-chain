#!/bin/bash

set -eu

usage()
{
        echo "Take a directory full of source code and make a reference to cpc-dev-tool-chain so that it can build it."
        echo
        echo "Usage: $0 (directoryname)*"
        echo
        echo "Directoryname can be just '.' to mean 'current directory'."
        echo
        echo "This script can be run on an existing project.  It will then update the"
        echo "reference to this copy of cpc-dev-tool-chain, keeping your parameters."
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
	echo "--------------------------------"
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
                        echo "Notice: found overwritable old generated Makefile, overwriting : $1"
                else
                        echo "There's already a makefile that I should probably not overwrite in: $1"
                        return 1
                fi
        fi

        {
                echo "# ${OVERWRITABLE_MARKER}"
                echo ""
                echo "# This Makefile links your project to the copy of cpc-dev-tool-chain to be used."
                echo "# It also provides assistance when none is found."
                echo "# This happens e.g. when this project is cloned to a new location."
                echo "# Please store this file along with your project files"
                echo "# in your version control repository (e.g. git)."
                echo ""
                echo "# cdtc_local_machine.conf is for stuff specific to this machine, like path to cpc-dev-tool-chain named CDTC_ROOT."
                echo "-include cdtc_local_machine.conf"
                echo ""
                echo "# sdcc-project.Makefile is the main default makefile inside cpc-dev-tool-chain."
                echo "-include \$(CDTC_ROOT)/sdcc-project.Makefile"
                echo ""
                echo "# sdcc-project.Makefile automatically includes, if they exist, the following:"
                echo "# * local.makefile is intended for your local custom makefiles rules (if any)."
                echo "# * cdtc_project.conf sets project parameters."
                echo ""
                echo "# You may define custom rules in your local.Makefile."
                echo "# The first rule if your local.Makefile will be called as default."
                echo ""
                echo "########################################################################"
                echo "# Below is fallback code: in case the main cpc-dev-tool-chain"
                echo "# directory was not found, explain what to do."
                echo ""
                echo "mkfile_path := \$(abspath \$(lastword \$(MAKEFILE_LIST)))"
                echo "current_dir := \$(dir \$(mkfile_path))"

                echo "fail_for_cannot_locate_cdtc:"
                echo -e "\011@echo 'Cannot locate cpc-dev-tool-chain main directory.'"
                echo -e "\011@echo 'Explanation: this project appears to rely on cpc-dev-tool-chain for compilation.'"
                echo -e "\011@echo 'You can get a copy from https://github.com/cpcitor/cpc-dev-tool-chain'"
                echo -e "\011@echo 'Then run:'"
                echo -e "\011@echo '/path/to/cpc-dev-tool-chain/cdtc-project-setup.sh \$(current_dir)'"
                echo -e "\011@echo 'Then you can run \"make\" here and the project should build.'"
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
                        echo "# Suggested parameters to put in this file if needed:"
                        echo "# CODELOC is base address on the CPC of the generated binary"
                        echo "# CODELOC=0x1000"
                        echo '# Override keyword below allows things like MAKE CFLAGS="-DNDEBUG=1"'
                        echo "# Ref https://stackoverflow.com/questions/2129391/append-to-gnu-make-variables-via-command-line"
                        echo "# override CFLAGS := -I\$(abspath \$(CDTC_ROOT)/cpclib/cfwi/include/) \$(CFLAGS)"
                        echo "# override LDLIBS := -l\$(abspath \$(CDTC_ENV_FOR_CFWI) ) \$(CFLAGS)"
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

        echo "Job done with: $1"
	echo "--------------------------------"
        return 0
}

echo "================================"
echo "${0##*/}: starting"
echo "Current dir is : $PWD"

while [ "$#" -gt 0 ]
do
        proceed_with_one_item "$1" || echo "Failed for $1"
        shift
done

echo "${0##*/}: clean exit."
echo "================================"
