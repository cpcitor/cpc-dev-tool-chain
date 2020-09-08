local-doc: coverage.html

coverage.html: all_fw_calls_official_list.csv firmware_coverage.sh cfwi.lib
	time bash firmware_coverage.sh

all_fw_calls_official_list.csv: s968se15.pdf
	pdfgrep '^[0-9][0-9]*:' "$<" | sed 's/^\([0-9]*\): \(.*[^ ]\) *#\(.*\)$$/\1,\2,\3/' >$@.tmp && mv -vf $@.tmp $@

s968se15.pdf:
	wget -S http://cpctech.cpcwiki.de/s968se15.pdf

local-clean:
	rm -f coverage.html all_fw_calls_official_list.csv
