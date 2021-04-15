SHELL=/bin/bash -eu

VARIABLES_AT_MAKEFILE_START := $(.VARIABLES)

CDTC_ROOT=..

.PHONY: run-tests-make run-tests-shell

all: all-tests summarize

all-tests: run-tests-make run-tests-shell

clean-tests:
	\rm -rfv */cap32_fortest.cfg */output */test-execution.log */test_verdict.txt */*~ */.build_dependencies_checked

summarize:
	@( set -o pipefail ; grep -h . */test_verdict.txt | sort | uniq -c >$@.tmp && mv -f $@.tmp test-result-summary.txt ; )
	@( set -o pipefail ; echo "########################################################################" ; if grep FAIL test-result-summary.txt ; then echo >&2 "At least one test fails" ; exit 1 ; else echo "ALL TEST PASS" ; exit 0 ; fi ; )



run-tests-shell: $(CDTC_ENV_FOR_CAPRICE32)
	@( set -o pipefail ; shopt -s nullglob ; rm -fv */output/*~ */model/*~ ; for TEST in */test.sh ; do echo "########################################################################" ; echo -ne "Shell TEST: $$TEST\t" ; bash $$TEST >$${MAKE}.log 2>&1 && echo "TEST PASS" || echo "TEST FAIL $$TEST" ; done ; exit 0 ; )

run-tests-make:
	@( set -o pipefail ; rm -fv */output/*~ */model/*~ ; for MAKE in */local.Makefile ; do echo "########################################################################" ; echo -ne "Make TEST: $$MAKE\t" ; ( cd "$$(dirname $$MAKE)" ; if { $(MAKE) test_verdict.txt ; diff -ur model output ; } >test-execution.log 2>&1 ; then echo "TEST PASS" ; else echo "TEST FAIL, see details in $$(dirname $$MAKE)/test-execution.log" ; fi ; ) ; done ; exit 0 ; )

########################################################################
# Run emulator
########################################################################

CDTC_ENV_FOR_CAPRICE32=$(CDTC_ROOT)/tool/caprice32/build_config.inc

$(CDTC_ENV_FOR_CAPRICE32): $(CDTC_ROOT)/tool/caprice32/Makefile
	( export LC_ALL=C ; $(MAKE) -C "$(@D)" build_config.inc ; )

debug:
	echo $(MAKES)

########################################################################
# Debug the makefile
########################################################################
#$(foreach v,                                        \
#  $(filter-out $(VARIABLES_AT_MAKEFILE_START) VARIABLES_AT_MAKEFILE_START,$(.VARIABLES)), \
#  $(info $(v) = $($(v))))
