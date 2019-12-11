SHELL=/bin/bash

#ALL_TOOLS=$(patsubst %/Makefile,%,$( wildcard tool/*/Makefile ) )
#ALL_CORE_TOOLS=$( for TOOL in $(ALL_TOOLS) ; do [[ -e "$TOOL"/.is_core_tool ]] && echo "$TOOL" ; done ; )

#ALL_CORE_TOOLS=$(shell ls -1b */.is_core_tool | sed 's|/.is_core_tool||' ; )
ALL_CORE_TOOLS=$(patsubst %/.is_core_tool,%,$(wildcard */.is_core_tool ) )
#ALL_CORE_TOOLS=$(wildcard */.is_core_tool)

test:
	echo all: $(ALL_CORE_TOOLS)

install: $(ALL_CORE_TOOLS)
	( set -xveu -o pipefail ; \
	export PREFIX=$$( cd ../.. ; echo $$PWD/cpc-dev-tool-chain_tools_installed ; ) ; \
	export PREFIX_BIN=$${PREFIX}/bin ; \
	for TOOL in $(ALL_CORE_TOOLS) ; \
	do make -C $$TOOL install ; \
	done ; )

# tool/*/Makefile: ; \
#     do \
#         TOOLD="${TOOLM%/*}" ; \
#         make -C "$TOOLD" ; \
#         for INC in "$TOOLD"/*.inc ; do echo ". $INC" >&4 ; done ; \
#     done 4> ../.tools_config.env
# RUN echo . ~/.tools_config.env >>~/.profile

# check:
# 	set -xv ; echo $ALL_TOOLS ; echo $ALL_CORE_TOOLS
