#!/bin/bash

GITHUB_CDTC=https://github.com/cpcitor/cpc-dev-tool-chain

########################################################################
# This script assumes:
# * sitting in the root directory of a project that depends on cpc-dev-tool-chain.

# This script produces:
# * a build tree with a makefile, ready to compile.

# This script does not guarantee:
# * that cpc-dev-tool-chain is fully compiled,etc.
# * that build will succeed.

function usage()
{
    ########################################################################
    [[ -n "$1" ]] && { echo >&2 "ERROR: $1" ; }
    cat <<EOF
########################################################################
This script configures a project that depends on cpc-dev-tool-chain
for compilation.

Usage:

./configure

    Inside project directory, just run without argument to
    automatically fetch dependencies and generate a build tree.
    The automatically generated path will be echoed.


./configure /path/to/cpc-dev-tool-chain

    If you already have a copy of cpc-dev-tool-chain that you want to
    use, add its path as argument.


mkdir build-tree
cd build-tree
$SCRIPTDIR/configure

    To customize build directory location and name, cd into it and run
    script.  (You can still add cpc-dev-tool-chain path as argument.)

########################################################################
EOF
    [[ -n "$1" ]] && {
        {
            echo "ERROR: $1" ;
            echo ########################################################################
            echo "Aborting."
        } >&2
        exit 1 ;
    }
}

########################################################################
# Stricter bash parameters, cf. https://news.ycombinator.com/item?id=8054440
set -eEuo pipefail
set -o noclobber
shopt -s shift_verbose

########################################################################
# Figure out what is where

# Current directory may become build tree destination
RUNDIR="$( set -e ; cd -P . ; pwd ; )"
echo "Current directory: $RUNDIR"

# Where is this script? This locates project directory.
SCRIPTDIR="$( set -e ; cd -P "$( set -e ; dirname "$0" )" ; pwd )"
echo "Project directory: $SCRIPTDIR"

########################################################################
# * check cpc-dev-tool-chain location, by order of priority
#     * path indicated on command-line
#     * else, see if git submodule is checked out
#     * else check it out

echo
echo "# Locating cpc-dev-tool-chain"

cd "$SCRIPTDIR"

function is_CDTC_ROOT_suitable()
{
    if [[ -z "${CDTC_ROOT:-}" ]]
    then
        #echo "CDTC_ROOT not set."
        return 1
    fi

    PROBE="${CDTC_ROOT}/sdcc-project.Makefile"
    [[ -r "$PROBE" ]]
}

function notice_if_CDTC_ROOT_unsuitable()
{
    PROBE="${CDTC_ROOT}/sdcc-project.Makefile"
    if [[ -r "$PROBE" ]]
    then
        return 0
    else
        echo "Does not contain a copy of cpc-dev-tool-chain: $CDTC_ROOT"
        echo "Moving on."
    fi
}

function inside_git_workdir()
{
    git rev-parse --is-inside-work-tree 2>/dev/null
}

if [[ $# -ge 1 ]]
then
    echo "Command line hints at CDTC_ROOT: $1"
    CDTC_ROOT="$( cd -P "$1" || exit 0 ; pwd ; )"
    if [[ -z "${CDTC_ROOT}" ]]
    then
        usage "Not a directory I can enter: $1"
        exit 1
    fi

    {
        cd $CDTC_ROOT
        PROBE="sdcc-project.Makefile"
        if ! [[ -r "$PROBE" ]]
        then
            usage "Not a path containing a copy of cpc-dev-tool-chain: $CDTC_ROOT"
        fi
    }
else
    echo "Path not set on command-line, probing for locations..."
fi

if ! is_CDTC_ROOT_suitable
then
    # Okay, no command line. Local config?
    if [[ -r cdtc_local_machine.conf ]]
    then
        CDTC_ROOT="$( set -e ; sed -n 's|^CDTC_ROOT *= *\(.*\)$|\1|p' <cdtc_local_machine.conf )"
        echo "Found in cdtc_local_machine.conf: $CDTC_ROOT"
        notice_if_CDTC_ROOT_unsuitable
    fi
fi

if ! is_CDTC_ROOT_suitable
then
    echo 'Checking `cpc-dev-tool-chain` subdirectory'

    PROBE="cpc-dev-tool-chain/sdcc-project.Makefile"
    if [[ -f "$PROBE" ]]
    then
        echo "Found $PROBE"
        CDTC_ROOT="$( cd -P "cpc-dev-tool-chain" ; pwd ; )"
    else
        echo "Probing did not find: $PROBE"

        echo "Using git to fetch cpc-dev-tool-chain from network..."
        if inside_git_workdir
        then
            echo "We're inside a GIT directory. Perhaps this repo has a suitable git submodule to populate cpc-dev-tool-chain. Trying..."
            set -xv
            if
                git submodule update --init -- cpc-dev-tool-chain
            then
                set +xv
                CDTC_ROOT="$( cd -P "cpc-dev-tool-chain" ; pwd ; )"
                echo "Fetched using git submodule: $CDTC_ROOT"
            else
                set +xv ; echo >&2 "git submodule update --init failed, moving along." ; break ;
            fi
        fi
        if ! [[ -f "$PROBE" ]]
        then
            echo "Mkay, git submodule option did not work.  Trying plain git clone."
            set -xv
            if
                git clone "$GITHUB_CDTC"
            then
                set +xv
                CDTC_ROOT="$( cd -P "cpc-dev-tool-chain" ; pwd ; )"
                echo "Created using git: $CDTC_ROOT"
            else
                echo >&2 "git clone failed, moving along." ; break ;
            fi
        fi
        if ! [[ -f "$PROBE" ]]
        then
            echo "Still not found. Mpfmfpfmmmf."
        fi
    fi

    notice_if_CDTC_ROOT_unsuitable
fi

# Final check
if is_CDTC_ROOT_suitable
then
    echo "Okay, located cpc-dev-tool-chain."
else
    echo >&2 "Failed to locate or fetch: $PROBE"
    echo >&2 "Please re-run this script, adding as parameter a path to cpc-dev-tool-chain"
    exit 1
fi

echo "CDTC_ROOT=$CDTC_ROOT"


########################################################################
# * check build tree location, by order of priority
#     * if current dir is outside of script directory, take that
#     * else generate a directory path

echo "Notice: currently ignoring build tree, since out-of-source build not yet done."
exec $CDTC_ROOT/cdtc-project-setup.sh $SCRIPTDIR
