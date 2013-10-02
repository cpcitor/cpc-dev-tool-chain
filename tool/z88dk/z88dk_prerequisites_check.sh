#!/bin/bash

set -eu

ERRORS=0

function log_failure()
{
        echo >&2 "$@"
        ERRORS=$((ERRORS + 1))
}

Z88DK_PERL_SCRIPT=z88dk/src/z80asm/tools/make_init.pl

if
        [[ -e "${Z88DK_PERL_SCRIPT}" ]]
then
        if
                perl "${Z88DK_PERL_SCRIPT}"
        then
                :
        else
                log_failure "error: missing dependency -- building z88dk assembler requires several modules."
                {
                        echo "You need at least (you may have some already):"
                        grep "^use" "${Z88DK_PERL_SCRIPT}"
                } >&2
        fi
else
        echo "No ${Z88DK_PERL_SCRIPT} no trouble."
fi

if
        echo "use Modern::Perl;" | perl
then
        :
else
        log_failure "error: missing dependency -- building z88dk assembler requires Modern::Perl."
fi

# z88Dk 1.10.1 does not yet require bison. Only CVS release does.
# if
#         which bison
# then
#         :
# else
#         log_failure "error: missing dependency -- building z88dk assembler requires bison."
# fi

echo "End of script. Errors: ${ERRORS}"

exit "${ERRORS}"
