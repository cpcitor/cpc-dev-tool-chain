.PHONY: run_test

test_verdict.txt: model output
	( if diff -ur model output ; then echo PASS ; else echo FAIL ; fi | tee $@.tmp && mv -vf $@.tmp $@ ; exit 0 )
# Make target should succeed even if test fails.

run_test output: cap32_fortest.cfg dsk
	( . $(CDTC_ENV_FOR_CAPRICE32) ; rm -rf output ; mkdir output ; cap32 $(DSKNAME) -c cap32_fortest.cfg -a 'run"$(PROJNAME)' -a CAP32_WAITBREAKCAP32_SCRNSHOTCAP32_EXIT ; rename 's|output/screenshot.*.png$$|output/screenshot.png|' output/screenshot*.png ; )



cap32_fortest.cfg: $(CDTC_ENV_FOR_CAPRICE32) local.Makefile
	{ sed \
		-e "s|speed=.*||" \
		-e "s|auto_pause=.*||" \
		-e "s|printer=.*|printer=1|" \
		-e "s|printer_file=.*|printer_file=output/parallel_port_log.txt|" \
		-e "s|sdump_dir=.*|sdump_dir=output|" \
		-e "s|scr_fps=.*|scr_fps=0|" \
		<$(CDTC_ROOT)/tool/caprice32/cap32_local.cfg ; \
	echo -e "speed=256\nlimit_speed=0\nauto_pause=0" ; } \
	>cap32_fortest.cfg

extra_clean: clean distclean
	rm -f cap32_fortest.cfg  test_result_raw.txt  test_verdict.txt
