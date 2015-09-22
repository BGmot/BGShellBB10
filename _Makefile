QMAKE_TARGET  = BGShellBB10
QMAKE         = $(QNX_HOST)/usr/bin/qmake
TARGET        = $(QMAKE_TARGET)

all: Makefile $(QMAKE_TARGET)

Simulator-Debug.bar: Simulator-Debug
	blackberry-nativepackager -devMode -package $@ bar-descriptor.xml -configuration Simulator-Debug

Device-Debug.bar: Device-Debug
	blackberry-nativepackager -devMode -package $@ bar-descriptor.xml -configuration Device-Debug

Device-Release.bar: Device-Release
	blackberry-nativepackager -package $@ bar-descriptor.xml -configuration Device-Release

clean:
	$(MAKE) -C ./arm -f Makefile sureclean
	$(MAKE) -C ./x86 -f Makefile sureclean
	$(RM) ./x86/o/.obj/*
	$(RM) ./x86/o-g/.obj/*
	$(RM) ./arm/o.le-v7/.obj/*
	$(RM) ./arm/o.le-v7-g/.obj/*

Makefile: FORCE
	$(QMAKE) -spec unsupported/blackberry-armv7le-qcc -o arm/Makefile $(QMAKE_TARGET).pro CONFIG+=device
	$(QMAKE) -spec unsupported/blackberry-x86-qcc -o x86/Makefile $(QMAKE_TARGET).pro CONFIG+=simulator

FORCE:

$(QMAKE_TARGET): device simulator

device:
	$(MAKE) -C ./arm -f Makefile all

Device-Debug: Makefile
	$(MAKE) -C ./arm -f Makefile debug

Device-Release: Makefile
	$(MAKE) -C ./arm -f Makefile release

simulator:
	$(MAKE) -C ./x86 -f Makefile all

Simulator-Debug: Makefile
	$(MAKE) -C ./x86 -f Makefile debug
	