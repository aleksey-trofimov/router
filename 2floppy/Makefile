# Makefile

KDI_NAME = router
CONFIG_NAME = $(KDI_NAME)
ifeq ($(shell cat $(CONFIG_NAME).config 2>/dev/null),)
CONFIG_NAME = $(KDI_NAME)_$(BLUECAT_TARGET_BSP)
endif

all     : rootfs kernel kdi

this    :
	cd src; make all

rootfs  : $(KDI_NAME).rfs $(KDI_NAME).tar

kernel	:
	mkkernel ./$(CONFIG_NAME).config ./$(KDI_NAME).kernel \
                 ./$(KDI_NAME).disk

kdi	: $(KDI_NAME).kdi

clean	:
	rm -f *.rfs *.tar *.kernel *.disk *.kdi router

xconfig :
	cp -f ./$(CONFIG_NAME).config $(BLUECAT_PREFIX)/usr/src/linux/.config; \
	(cd $(BLUECAT_PREFIX)/usr/src/linux; \
	make xconfig); \
	cp -f $(BLUECAT_PREFIX)/usr/src/linux/.config ./$(CONFIG_NAME).config

%.rfs : %.spec
	mkrootfs -lv  ./$(KDI_NAME).spec ./$(KDI_NAME).rfs

%.tar : %.spec
	mkrootfs -lvT ./$(KDI_NAME).spec ./$(KDI_NAME).tar

%.kernel : $(CONFIG_NAME).config
	mkkernel ./$(CONFIG_NAME).config ./$(KDI_NAME).kernel ./$(KDI_NAME).disk

$(KDI_NAME).kdi : $(KDI_NAME).rfs $(KDI_NAME).disk
	mkboot -m -k $(KDI_NAME).disk -f $(KDI_NAME).rfs $(KDI_NAME).kdi

# end of file
