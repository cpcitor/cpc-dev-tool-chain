doc: all_fw_calls_official_list.csv

all_fw_calls_official_list.csv: s968se15.pdf
	pdfgrep '^[0-9][0-9]*:' "$<" | sed 's/^\([0-9]*\): \(.*[^ ]\) *#\(.*\)$$/\1,\2,\3/' >$@.tmp && mv -vf $@.tmp $@

s968se15.pdf:
	wget -S http://cpctech.cpc-live.com/s968se15.pdf
