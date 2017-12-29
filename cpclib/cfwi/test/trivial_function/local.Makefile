test_verdict.txt: test_result_raw.txt
	( if diff test_result_raw.txt test_result_reference.txt ; then echo PASS ; else echo FAIL ; fi | tee $@.tmp && mv -vf $@.tmp $@ ; )
# Make target should succeed even if test fails.

test_result_raw.txt: cap32_fast.cfg dsk
	( . $(CDTC_ENV_FOR_CAPRICE32) ; export SDL_VIDEODRIVER=dummy ; cap32_once $(DSKNAME) -c cap32_fast.cfg -a 'run"$(PROJNAME)' && mv -vf printer.dat $@ )

cap32_fast.cfg: $(CDTC_ENV_FOR_CAPRICE32) local.Makefile
	sed -e "s|speed=.*|speed=256|" -e "s|printer=.*|printer=1|" <$(CDTC_ROOT)/tool/caprice32/cap32_local.cfg >cap32_fast.cfg

extra_clean: clean distclean
	rm -f cap32_fast.cfg  test_result_raw.txt  test_verdict.txt
