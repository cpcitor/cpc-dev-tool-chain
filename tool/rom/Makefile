SHELL=/bin/bash

VARIABLES_AT_MAKEFILE_START := $(.VARIABLES)

TARGETS=cpc6128.rom

roms/cpc6128.rom: roms/OS_6128.ROM roms/BASIC_1.1.ROM
	cat $^ >$@

# URLS from http://www.cpcwiki.eu/index.php/ROM_List

ROMS_6128_URL=http://www.cpcwiki.eu/imgs/c/c6/OS_6128.zip http://www.cpcwiki.eu/imgs/f/fb/BASIC_1.1.zip $(AMSDOS_URL)
AMSDOS_URL=http://www.cpcwiki.eu/imgs/7/7d/AMSDOS_0.5.zip

ROMS_URL=$(ROMS_6128_URL)

all: $(TARGETS)

########################################################################
# Variables to adjust at each new integrated project
########################################################################

# You must set EXTRACT_DIR_NAME to match archive content.
FETCH_DIR_NAME=rom_zips
EXTRACT_DIR_NAME=roms

# If only one generated executable file, set BUILD_TARGET_FILE to path to it.
# It several generated executable files, do not set BUILD_TARGET_FILE.
BUILD_TARGET_FILE=$(EXTRACT_DIR_NAME)/cap32

URL_DOWNLOAD=$(URL_GIT)

.PHONY: fetched

#fetched: $(FETCH_DIR_NAME)/.fetched
extracted: $(EXTRACT_DIR_NAME)/.extracted Makefile

$(FETCH_DIR_NAME)/.fetched:
	( set -euxv ; mkdir -p "$(FETCH_DIR_NAME)" ; RUND="$$PWD" ; cd "$(FETCH_DIR_NAME)" ; bash "$${RUND}/download_roms.sh" $(ROMS_URL) ; )
	@echo "************************************************************************"
	@echo "**************** Rom archives were downloaded to: $(@D)"
	@echo "************************************************************************"
	touch $@

# ZIPD="$$( cd "$(FETCH_DIR_NAME)" ; pwd )" ; 
roms/OS_6128.ROM roms/BASIC_1.1.ROM $(EXTRACT_DIR_NAME)/.extracted: $(FETCH_DIR_NAME)/.fetched
	( set -euxv ; mkdir -p "$(EXTRACT_DIR_NAME)" ; MAKED="$$PWD" ; cd "$(EXTRACT_DIR_NAME)" ; for a in $$(find $$MAKED/$(FETCH_DIR_NAME) -iname "*.zip") ; do unzip -o "$$a" ; done ; )
	@echo "************************************************************************"
	@echo "**************** Rom archives were downloaded to: $(@D)"
	@echo "************************************************************************"
	touch $@

clean:
	-rm -f $(BUILD_TARGET_FILE)

mrproper:
	-rm -f $(BUILD_TARGET_FILE)
	-rm -f $(TARGETS)
	-rm -rf $(EXTRACT_DIR_NAME) ._$(EXTRACT_DIR_NAME) *~
	-rm -fr $(PRODUCT_NAME) $(EXTRACT_DIR_NAME) $(FETCH_DIR_NAME)

distclean: mrproper
	-rm -fr $(ARCHIVE_NAME)

########################################################################
# Debug the makefile
########################################################################
$(foreach v,                                        \
  $(filter-out $(VARIABLES_AT_MAKEFILE_START) VARIABLES_AT_MAKEFILE_START,$(.VARIABLES)), \
  $(info $(v) = $($(v))))


## Other, not used
# 
# ROMS_464_EN_URL=http://www.cpcwiki.eu/imgs/0/0f/OS_464.zip http://www.cpcwiki.eu/imgs/4/4a/BASIC_1.0.zip $(AMSDOS_URL)
# ROMS_6128_EN_URL=http://www.cpcwiki.eu/imgs/c/c6/OS_6128.zip http://www.cpcwiki.eu/imgs/f/fb/BASIC_1.1.zip $(AMSDOS_URL)
# 
# ROMS_464_FR_URL=http://www.cpcwiki.eu/imgs/4/4d/Os464_%28French_AZERTY%29.rom $(AMSDOS_URL)
# ROMS_6128_FR_URL=http://www.cpcwiki.eu/imgs/a/af/French_6128_ROMs.zip $(AMSDOS_URL)
# 
# ROMS_464_ES_URL=http://www.cpcwiki.eu/imgs/1/18/Os464_%28Spanish%29.rom $(AMSDOS_URL)
# ROMS_6128_ES_URL=http://www.cpcwiki.eu/imgs/6/6a/Spanish_6128_ROMs.zip http://www.cpcwiki.eu/imgs/a/ae/BASIC1.1_%286128_Spanish%29.ROM $(AMSDOS_URL)
# 
# AMSDOS_URL=http://www.cpcwiki.eu/imgs/7/7d/AMSDOS_0.5.zip

#ROMS_URL=$(ROMS_464_EN_URL) $(ROMS_6128_EN_URL) $(ROMS_464_FR_URL) $(ROMS_6128_FR_URL) $(ROMS_464_ES_URL) $(ROMS_6128_ES_URL)
