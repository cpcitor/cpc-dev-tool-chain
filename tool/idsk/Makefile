TARGETS=build_config.inc

all: $(TARGETS)

########################################################################

PRODUCT_NAME=iDSK
URL_RELEASE=http://sid.cpc.free.fr/iDSK.0.13-src.tgz
#ARCHIVE_NAME=...whatever or
EXTRACT_DIR_NAME=iDSK.0.13
BUILD_TARGET_FILE=$(EXTRACT_DIR_NAME)/iDSK/src/iDSK

URL_DOWNLOAD=$(URL_RELEASE)

ARCHIVE_NAME?=$(notdir $URL_DOWNLOAD)

$(ARCHIVE_NAME):
	wget -S $(URL_RELEASE) -O $@.tmp
	mv -vf $@.tmp $@
	@echo "************************************************************************"
	@echo "**************** Source archive was downloaded to: $(@)"
	@echo "************************************************************************"

$(EXTRACT_DIR_NAME)/.unpacked: $(ARCHIVE_NAME)
	@echo "************************************************************************"
	@echo "**************** Extracting source from: $^"
	@echo "************************************************************************"
	tar zxvf $<
	touch $@
	@echo "************************************************************************"
	@echo "**************** Source extracted to: $(@D)"
	@echo "************************************************************************"

PATCHNAME=$(EXTRACT_DIR_NAME)_build_patch.patch

ifeq ($(wildcard $(PATCHNAME)),)
$(EXTRACT_DIR_NAME)/.patched: $(PATCHNAME) $(EXTRACT_DIR_NAME)/.unpacked
	@echo "************************************************************************"
	@echo "**************** No patch to apply to: $^"
	@echo "************************************************************************"
	touch $@
	@echo "************************************************************************"
	@echo "**************** Source patched in: $(@D)"
	@echo "************************************************************************"
else
$(EXTRACT_DIR_NAME)/.patched: $(PATCHNAME) $(EXTRACT_DIR_NAME)/.unpacked
	@echo "************************************************************************"
	@echo "**************** Patching source in: $^"
	@echo "************************************************************************"
	patch -p0 < $<
	touch $@
	@echo "************************************************************************"
	@echo "**************** Source patched in: $(@D)"
	@echo "************************************************************************"
endif

BUILD_TARGET_FILE?=$(EXTRACT_DIR_NAME)/.built

$(BUILD_TARGET_FILE): $(EXTRACT_DIR_NAME)/.patched
	@echo "************************************************************************"
	@echo "**************** Configuring and build in: $^"
	@echo "************************************************************************"
	( set -e ; cd $(EXTRACT_DIR_NAME)/iDSK ; ./configure ; LC_ALL=C $(MAKE) --print-directory ; )
	@echo "************************************************************************"
	@echo "**************** Configured and built in: $(@D)"
	@echo "************************************************************************"

build_config.inc: $(BUILD_TARGET_FILE) Makefile
	(set -eu ; \
	{ \
	echo "# with bash do \"source\" this file." ; \
	cd "$(<D)" ; \
	echo "export PATH=\"\$${PATH}:$$PWD\"" ; \
	} >$@ ; )

$(PRODUCT_NAME): $(BUILD_TARGET_FILE)
	cp -avf $< $@

clean:
	-( set -e ; cd $(EXTRACT_DIR_NAME)/iDSK ; $(MAKE) clean ; )
	-rm -f $(BUILD_TARGET_FILE)

mrproper: clean
	-rm -rf $(EXTRACT_DIR_NAME) ._$(EXTRACT_DIR_NAME) *~ build_config.inc

distclean: mrproper
	-rm -f $(PRODUCT_NAME) $(EXTRACT_DIR_NAME)
